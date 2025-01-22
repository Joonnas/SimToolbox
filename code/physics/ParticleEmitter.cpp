#include "ParticleEmitter.h"

#include <stdlib.h>
#include <math.h>

#include "ext/include/GLFW/glfw3.h"

#include "gui/Camera.h"

// The function idhash is from the Godot CPUParticle 2D implementation
static uint32_t idhash(uint32_t x) {
	x = ((x >> uint32_t(16)) ^ x) * uint32_t(0x45d9f3b);
	x = ((x >> uint32_t(16)) ^ x) * uint32_t(0x45d9f3b);
	x = (x >> uint32_t(16)) ^ x;
	return x;
}

float ParticleEmitter::vertices[18] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
};

void ParticleEmitter::Particle::update(float deltaTime, aMath::Vec3 res_force=aMath::Vec3()) {
    if (res_force.length_squared() > 0.0f) {
        velocity = velocity + (res_force * deltaTime); // res_force = (res_force / mass) but since mass does not exist we assume it as 1
    }
    pos = pos + (velocity * deltaTime);
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        alive = false;
    }
    // DEBUG("x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z);
}


void ParticleEmitter::prepare_particles() {
    // Make vector smaller 
    if (particles.size() > *amount) {
        for (int i = *amount; i < particles.size(); i++) {
            delete particles[i];
        }
        particles.resize(*amount);
    }
    // Make vector bigger
    else if (particles.size() < *amount) {
        int old_amount = particles.size();
        particles.reserve(*amount);
        for (int i = old_amount; i < *amount; i++) {
            Particle* p = new Particle(aMath::Vec3(), aMath::Vec3(), *particle_lifetime);
            particles.push_back(p);
        }
    }
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->set_alive(false);
    }
}


void ParticleEmitter::spawnParticle(int i, float delta) {
    particles[i]->set_delta(delta);

    // set velocity based on spread
    aMath::Vec3 spawn_velocity = initial_velocity->get();
    if (*spread > 0.0) {
        float rand_spread = aMath::TAU * ((rand() % 1000) / 1000.0) * *spread;
        float max_spread = (aMath::TAU * *spread) / 2.0;
        aMath::Mat3 rot_matrix = aMath::Mat3();
        rot_matrix.rotate(rand_spread - max_spread);
        spawn_velocity = mul(rot_matrix, spawn_velocity);
    }
    particles[i]->set_velocity(spawn_velocity);

    // set position based on emission shape
    aMath::Vec3 spawn_position = pos->get();
    if (emission_shape->get() == EmissionShapes::SHAPE_SPHERE) {
        float phase = aMath::TAU * ((rand() % 100) / 100.0);
        float radius = ((rand() % 100) / 100.0) * *sphere_radius;
        aMath::Vec2 dir = aMath::Vec2(cos(phase), sin(phase)) * radius;
        spawn_position.x += dir.x;
        spawn_position.y += dir.y;
    } else if(emission_shape->get() == EmissionShapes::SHAPE_RECT) {
        float x_add = ((rand() % 100) / 100.0) * rect_extents->get().x - (rect_extents->get().x / 2.0);
        float y_add = ((rand() % 100) / 100.0) * rect_extents->get().y - (rect_extents->get().y / 2.0);
        spawn_position.x += x_add;
        spawn_position.y += y_add;
    }
    particles[i]->set_pos(spawn_position);
    
    particles[i]->set_alive(true);
    particles[i]->set_lifetime(*particle_lifetime);
}


void ParticleEmitter::reset() {
    prepare_particles();
    time = 0.0f;
    cycle = 0;
}



void ParticleEmitter::update(float deltaTime) {

    if (*emitting || time > 0.0) time += deltaTime;
    if (!*emitting) {
        cycle = 0;
        cycle_start_time = time;
    }

    if (time > *particle_lifetime) {
        cycle_start_time = 0.0f;
        cycle++;
        time -= *particle_lifetime;
        if(*oneshot) {
            emitting->set(false);
        }
        if (!*emitting) time = 0.0f;
    }

    for (int i = 0; i < particles.size(); i++) {
        if (particles[i]->is_alive()) {
            particles[i]->update(deltaTime, *constant_acceleration);
        }
        if (*emitting && !particles[i]->is_alive()) {

            float restart_phase = float(i) / float(*amount);

            // randomness and explosiveness are from the Godot CPUParticle 2D implementation
            // add randomness
            if (*lifetime_randomness > 0.0) {
                uint32_t seed = cycle;
                seed *= uint32_t(*amount);
                seed += uint32_t(i);
                float random = float(idhash(seed) % uint32_t(65536)) / 65536.0f;
                restart_phase += *lifetime_randomness * random * 1.0/float(*amount);
            }

            // add explosiveness
            restart_phase *= (1.0 - *explosiveness);
            float restart_time = restart_phase * *particle_lifetime;

            // spawn if the start time
            if (restart_time >= cycle_start_time && restart_time < time) {
                spawnParticle(i, time - restart_time);
            }

        }
    }
}

void ParticleEmitter::render() {
    glUseProgram(shaderProgram);
    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "mvp_matrix");
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 1.0, 0.0, 0.0);
    for (int i = 0; i < particles.size(); i++) {
        // only render particles that are alive
        if (!particles[i]->is_alive())
            continue;
        aMath::Mat4 mvp;
        aMath::Mat4 model;

        // create model matrix
        model = aMath::mul(model, aMath::translate(particles[i]->get_pos()));
        model = aMath::mul(model, aMath::scale(0.02, 0.02, 0.02));

        // construct mvp matrix
        mvp = aMath::mul(Camera::get_camera_singleton()->get_projection_matrix(), Camera::get_camera_singleton()->get_camera_matrix());
        mvp = aMath::mul(mvp, model);

        // send information to opengl
        glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.values);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

ParticleEmitter::ParticleEmitter(Property<aMath::Vec3>* p_pos, Property<bool>* p_emitting, Property<bool>* p_oneshot, Property<int>* p_amount, Property<int>* p_emission_shape, Property<float>* p_sphere_radius, Property<aMath::Vec2>* p_rect_extents, Property<float>* p_explosiveness, Property<aMath::Vec3>* p_initial_velocity, Property<float>* p_spread, Property<aMath::Vec3>* p_constant_acceleration, Property<float>* p_particle_lifetime, Property<float>* p_lifetime_randomness) :
    pos(p_pos), emitting(p_emitting), oneshot(p_oneshot), amount(p_amount), emission_shape(p_emission_shape), sphere_radius(p_sphere_radius), rect_extents(p_rect_extents), explosiveness(p_explosiveness), initial_velocity(p_initial_velocity), spread(p_spread), constant_acceleration(p_constant_acceleration), particle_lifetime(p_particle_lifetime), lifetime_randomness(p_lifetime_randomness) {
    
    // OPENGL RENDERING

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 mvp_matrix;\n"
        "void main() {\n"
        "   gl_Position = mvp_matrix * vec4(aPos, 1.0);\n"
        "}\0";
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        ERROR("ParticleEmitter -- VERTEX: " << infoLog);
    }

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main() {\n"
        // "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        ERROR("ParticleEmitter -- FRAG: " << infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        ERROR("ParticleEmitter -- LINK: " << infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleEmitter::vertices), ParticleEmitter::vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // END OPENGL RENDERING

    srand(glfwGetTime());

    // set reset function so that it is called upon any change
    amount->set_onChange([this](int) {this->reset();});
    explosiveness->set_onChange([this](float) {this->reset();});
    particle_lifetime->set_onChange([this](float) {this->reset();});
    lifetime_randomness->set_onChange([this](float) {this->reset();});

    reset();
}

ParticleEmitter::~ParticleEmitter() {
    for (int i = 0; i < particles.size(); i++) {
        delete particles[0];
        particles.erase(particles.begin());
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}