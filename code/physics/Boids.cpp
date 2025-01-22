#include "Boids.h"

#include <stdlib.h>
#include <math.h>

#include "ext/include/GLFW/glfw3.h"

#include "gui/Camera.h"
#include "Utils/draw.h"

float Boids::vertices[9] = {
    -0.5f, 0.5f, 0.0f,
     0.5f,  0.0f, 0.0f,
     -0.5f,  -0.5f, 0.0f,
};

void Boids::BoidData::update(float deltaTime, aMath::Vec3 forces, float p_max_speed, aMath::Vec4 bounds) {
    // update velocity
    // and scale it if it exceeds the max_speed
    velocity = velocity + (forces * deltaTime);
    if (velocity.length() > p_max_speed) velocity.scale(p_max_speed);
    
    // update position
    // and take borders into account
    pos = pos + (velocity * deltaTime);
    if (pos.x > bounds.x + bounds.z) pos.x -= bounds.z;
    if (pos.x < bounds.x) pos.x += bounds.z;
    if (pos.y > bounds.y + bounds.w) pos.y -= bounds.w;
    if (pos.y < bounds.y) pos.y += bounds.w; 

    // update rotation if velocity is greater 0
    if (velocity.length() > 0.0) {
        aMath::Vec3 dir_normal = velocity;
        dir_normal.normalize();
        float rot = asin(abs(dir_normal.y) / dir_normal.length());
        if (velocity.x > 0.0 && velocity.y > 0.0) { // upper right quadrant
            rotation = rot;
        } else if (velocity.x < 0.0 && velocity.y > 0.0) { // upper left quadrant
            rotation = aMath::PI - rot;
        } else if(velocity.x > 0.0 && velocity.y < 0.0) { // lower right quadrant
            rotation = aMath::TAU - rot;
        } else { // velocity.x < 0.0 && velocity.y < 0.0 // lower left quadrant
            rotation = aMath::PI + rot;
        }
    }
}

aMath::Vec3 Boids::seperate(BoidData* current, std::vector<BoidData*> boids_in_view) {
    aMath::Vec3 steer;
    int count = 0;

    // Sum over boids in view of displacement / distance squared between the boids
    for (int i = 0; i < boids_in_view.size(); i++) {
        if (current == boids_in_view[i]) continue;
        aMath::Vec3 displace = current->get_pos() - boids_in_view[i]->get_pos();
        float distance = displace.length_squared();
        steer = steer + (displace / ((distance != 0.0) ? distance : 0.01)); // Avoid division by zero
        count += 1;
    }

    // if there are boids in the perception don't apply the full force,
    // but make it a turning force and scale it
    if (count > 0) {
        steer = steer - current->get_velocity();
        if (steer.length() > 1.0) steer.scale(1.0);
    }

    return steer;
}
aMath::Vec3 Boids::align(BoidData* current, std::vector<BoidData*> boids_in_view) {
    aMath::Vec3 steer;
    int count = 0;

    // Sum over boids in view of the velocities
    for (int i = 0; i < boids_in_view.size(); i++) {
        if (current == boids_in_view[i]) continue;
        steer = steer + boids_in_view[i]->get_velocity();
        count += 1;
    }

    if (count > 0) {
        // Average the velocities
        // steer = steer / count;
        steer = steer - current->get_velocity();
        if (steer.length() > 1.0) steer.scale(1.0);
    }

    return steer;
}
aMath::Vec3 Boids::cohese(BoidData* current, std::vector<BoidData*> boids_in_view) {
    aMath::Vec3 steer;
    aMath::Vec3 average_position;
    int N = boids_in_view.size();

    // Sum over boids in view of the scaled position (xi / N) - x
    for (int i = 0; i < boids_in_view.size(); i++) {
        // if (current == boids_in_view[i]) continue; -> take current boid into account to avoid N == 0
        average_position = average_position + ((boids_in_view[i]->get_pos() / N) - current->get_pos());
    }

    if (N > 0) {
        steer = average_position - current->get_pos();
        steer = steer - current->get_velocity();
        if (steer.length() > 1.0) steer.scale(1.0);
    }

    return steer;
}
aMath::Vec3 Boids::additionalForces(BoidData* current, std::vector<BoidData*> boids_in_view) {
    return aMath::Vec3(0.0, 0.0, 0.0);
    // aMath::Vec3 collision_forces;

    // aMath::Vec3 nl = aMath::Vec3(1.0, 0.0, 0.0);
    // aMath::Vec3 nr = aMath::Vec3(-1.0, 0.0, 0.0);
    // aMath::Vec3 nt = aMath::Vec3(0.0, 1.0, 0.0);
    // aMath::Vec3 nb = aMath::Vec3(0.0, -1.0, 0.0);

    // collision_forces = nl / (current->get_pos() - aMath::Vec3(-custom_boundary_extents->get().x / 2.0, current->get_pos().y, 0.0)).length();
    // collision_forces = collision_forces + (nr / (current->get_pos() - aMath::Vec3(custom_boundary_extents->get().x / 2.0, current->get_pos().y, 0.0)).length());
    // collision_forces = collision_forces + (nt / (current->get_pos() - aMath::Vec3(current->get_pos().x, -custom_boundary_extents->get().y / 2.0, 0.0)).length());
    // collision_forces = collision_forces + (nb / (current->get_pos() - aMath::Vec3(current->get_pos().x, custom_boundary_extents->get().y / 2.0, 0.0)).length());
    
    // collision_forces = collision_forces * 0.01;

    // return collision_forces;
}

void Boids::prepareBoids() {
    time = 0.0f;
    pos_ul = aMath::Vec2(pos->get().x - custom_boundary_extents->get().x, pos->get().y - custom_boundary_extents->get().y);
    size = aMath::Vec2(custom_boundary_extents->get().x * 2.0, custom_boundary_extents->get().y * 2.0);

    if (boid_data.size() > *amount) boid_data.resize(*amount);
    else {
        int old_amount = boid_data.size();
        boid_data.reserve(*amount);
        for (int i = old_amount; i < *amount; i++) {
            float spawn_pos_x = pos->get().x;
            float x_add = ((rand() % 1000) / 1000.0) * custom_boundary_extents->get().x * 2.0 - (custom_boundary_extents->get().x);
            float spawn_pos_y = pos->get().y;
            float y_add = ((rand() % 1000) / 1000.0) * custom_boundary_extents->get().y * 2.0 - (custom_boundary_extents->get().y);
            float vel_x = ((rand() % 1000) / 1000.0) * 0.2 - 0.1;
            float vel_y = ((rand() % 1000) / 1000.0) * 0.2 - 0.1;
            float rot = ((rand() % 1000) / 1000.0) * aMath::TAU;
            BoidData* bd = new BoidData(aMath::Vec3(spawn_pos_x + x_add, spawn_pos_y + y_add, 0.0), aMath::Vec3(0.0, 0.0, 0.0), rot);
            boid_data.push_back(bd);
        }
    }
}

void Boids::rebuildQTree() {
    qtree = QuadTree<BoidData*>(pos_ul, size);
    for (int i = 0; i < *amount; i++) {
        qtree.insert(aMath::Vec2(boid_data[i]->get_pos().x, boid_data[i]->get_pos().y), boid_data[i]);
    }
}

void Boids::update(float deltaTime) {
    time += deltaTime;
    if (time > 0.2f) {
        rebuildQTree();
        time -= 0.2f;
    } 

    for (int i = 0; i < boid_data.size(); i++) {
        aMath::Vec3 res_forces;

        //Calculate view angle and query the boids in view
        std::vector<BoidData*> boid_data_in_view;
        if (boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_CENTERED_RECT || boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_FOLLOWING_RECT) {
            aMath::Vec2 view_rect_ul;

            if (boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_CENTERED_RECT) {
                view_rect_ul = aMath::Vec2(boid_data[i]->get_pos().x - bpf_rect_size->get().x / 2.0, boid_data[i]->get_pos().y - bpf_rect_size->get().y / 2.0);
            } else if (boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_FOLLOWING_RECT) {
                aMath::Vec3 normalized_boid_velocity = boid_data[i]->get_velocity();
                normalized_boid_velocity.normalize();
                view_rect_ul = aMath::Vec2(boid_data[i]->get_pos().x - bpf_rect_size->get().x / 2.0, boid_data[i]->get_pos().y - bpf_rect_size->get().y / 2.0) + (aMath::Vec2(normalized_boid_velocity.x, normalized_boid_velocity.y) * (bpf_rect_size->get() / 2.0));
            }

            boid_data_in_view = qtree.get_in_rectangle(view_rect_ul, bpf_rect_size->get());
       
        } else if (boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_CIRCULAR_ARC) {
            // TODO: Calculate circular perception field
        }
        
        // Calculate the res_force
        res_forces = res_forces + seperate(boid_data[i], boid_data_in_view) * seperation->get();
        res_forces = res_forces + align(boid_data[i], boid_data_in_view) * alignment->get();
        res_forces = res_forces + cohese(boid_data[i], boid_data_in_view) * cohesion->get();
        res_forces = res_forces + additionalForces(boid_data[i], boid_data_in_view);
        if (res_forces.length() > 1.0) res_forces.scale(1.0);

        // Update the boid data
        boid_data[i]->update(deltaTime, res_forces, max_speed->get(), aMath::Vec4(pos_ul.x, pos_ul.y, size.x, size.y));
    }
}

void Boids::render() {
    glUseProgram(shaderProgram);
    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "mvp_matrix");
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.0, 1.0, 0.0);

    for (int i = 0; i < boid_data.size(); i++) {

        aMath::Mat4 mvp;
        aMath::Mat4 model;

        // create model matrix
        model = aMath::mul(model, aMath::translate(boid_data[i]->get_pos()));
        model = aMath::mul(model, aMath::rotateZ(boid_data[i]->get_rotation()));
        model = aMath::mul(model, aMath::scale(0.02, 0.02, 0.02));

        // construct mvp matrix
        mvp = aMath::mul(Camera::get_camera_singleton()->get_projection_matrix(), Camera::get_camera_singleton()->get_camera_matrix());
        mvp = aMath::mul(mvp, model);

        glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.values);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glBindVertexArray(0);
    glUseProgram(0);

    // Draw Visual Field
    if (draw_boid_visual_field->get() && amount->get() > 0 && boid_perception_field->get() != Boids::BoidPerceptionField::BOID_PERCEPTION_CIRCULAR_ARC) {

        aMath::Vec2 view_rect_ul;

        if (boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_CENTERED_RECT) {
            view_rect_ul = aMath::Vec2(boid_data[0]->get_pos().x - bpf_rect_size->get().x / 2.0, boid_data[0]->get_pos().y - bpf_rect_size->get().y / 2.0);
        } else if (boid_perception_field->get() == Boids::BoidPerceptionField::BOID_PERCEPTION_FOLLOWING_RECT) {
            aMath::Vec3 normalized_boid_velocity = boid_data[0]->get_velocity();
            normalized_boid_velocity.normalize();
            view_rect_ul = aMath::Vec2(boid_data[0]->get_pos().x - bpf_rect_size->get().x / 2.0, boid_data[0]->get_pos().y - bpf_rect_size->get().y / 2.0) + (aMath::Vec2(normalized_boid_velocity.x, normalized_boid_velocity.y) * (bpf_rect_size->get() / 2.0));
        }


        DrawPen::get_drawpen_singleton()->set_fill_color(aMath::Color_RGBA(0.0, 1.0, 0.0, 0.2));
        DrawPen::get_drawpen_singleton()->set_draw_color(aMath::Color_RGBA(1.0, 0.0, 0.0, 1.0));
        DrawPen::get_drawpen_singleton()->set_stroke_size(0.25);
        DrawPen::get_drawpen_singleton()->fillRect(view_rect_ul, bpf_rect_size->get(), true);
    }

    // Draw Velocities
    if (draw_boid_velocity->get()) {
        DrawPen::get_drawpen_singleton()->set_stroke_size(0.1);
        DrawPen::get_drawpen_singleton()->set_draw_color(aMath::Color_RGBA(0.0, 0.0, 1.0, 1.0));
        for (int i = 0; i < boid_data.size(); i++) {
            aMath::Vec2 boid_pos = aMath::Vec2(boid_data[i]->get_pos().x, boid_data[i]->get_pos().y);
            aMath::Vec2 boid_vel_scaled = aMath::Vec2(boid_data[i]->get_velocity().x, boid_data[i]->get_velocity().y);
            
            float remap_factor = boid_vel_scaled.length() / max_speed->get();
            remap_factor = (remap_factor > 1.0) ? 1.0 : ((remap_factor < 0.0) ? 0.0 : remap_factor);
            boid_vel_scaled.scale(remap_factor * 0.2);
            
            aMath::Vec2 boid_pos_plus_velocity = boid_pos + boid_vel_scaled;
            DrawPen::get_drawpen_singleton()->drawLine(boid_pos, boid_pos_plus_velocity);
        }
    }

    // Draw Quadtree
    if (draw_quadtree->get()) qtree.debug_draw();

    // Draw border
    if (draw_border->get()) {
        DrawPen::get_drawpen_singleton()->set_stroke_size(0.25);
        DrawPen::get_drawpen_singleton()->set_draw_color(aMath::Color_RGBA(0.0, 1.0, 0.0, 0.2));
        DrawPen::get_drawpen_singleton()->drawRect(aMath::Vec2(pos->get().x - custom_boundary_extents->get().x, pos->get().y - custom_boundary_extents->get().y), aMath::Vec2(custom_boundary_extents->get().x * 2.0, custom_boundary_extents->get().y * 2.0));
    }
}

Boids::Boids(Property<aMath::Vec3>* p_pos, Property<int>* p_amount, Property<aMath::Vec2>* p_custom_boundary_extents, Property<int>* p_boid_perception_field, Property<aMath::Vec2>* p_bpf_rect_size, Property<float>* p_bpf_circ_radius, Property<float>* p_bpf_circ_arc_angle, Property<float>* p_max_speed, Property<float>* p_seperation, Property<float>* p_alignment, Property<float>* p_cohesion, Property<bool>* p_draw_quadtree, Property<bool>* p_draw_border, Property<bool>* p_draw_boid_visual_field, Property<bool>* p_draw_boid_velocity) :
    pos(p_pos), amount(p_amount), custom_boundary_extents(p_custom_boundary_extents), boid_perception_field(p_boid_perception_field), bpf_rect_size(p_bpf_rect_size), bpf_circ_radius(p_bpf_circ_radius), bpf_circ_arc_angle(p_bpf_circ_arc_angle), max_speed(p_max_speed), seperation(p_seperation), alignment(p_alignment), cohesion(p_cohesion), draw_quadtree(p_draw_quadtree), draw_border(p_draw_border), draw_boid_visual_field(p_draw_boid_visual_field), draw_boid_velocity(p_draw_boid_velocity), qtree(QuadTree<BoidData*>(aMath::Vec2(0.0, 0.0), aMath::Vec2(0.0, 0.0))) {

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
        ERROR("Boids -- VERTEX: " << infoLog);
    }

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main() {\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        ERROR("Boids -- FRAG: " << infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        ERROR("Boids -- LINK: " << infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Boids::vertices), Boids::vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // END OPENGL RENDERING

    srand(glfwGetTime());

    pos->set_onChange([this](aMath::Vec3) {this->prepareBoids();});
    amount->set_onChange([this](int) {this->prepareBoids();});
    custom_boundary_extents->set_onChange([this](aMath::Vec2) {this->prepareBoids();});

    prepareBoids();
    rebuildQTree();

}

Boids::~Boids() {
    for (int i = 0; i < boid_data.size(); i++) {
        delete boid_data[i];
        boid_data.erase(boid_data.begin());
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}