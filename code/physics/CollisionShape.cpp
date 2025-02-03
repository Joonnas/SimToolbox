#include "CollisionShape.h"

#include "physics/PhysicsServer.h"
#include "gui/Camera.h"

float CollisionShape::vertices_circle[180] = {
       0.0f,    0.0f, 0.0f,
       0.0f,    0.5f, 0.0f,
     0.154f,  0.475f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.154f,  0.475f, 0.0f,
     0.293f,  0.404f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.293f,  0.404f, 0.0f,
     0.404f,  0.293f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.404f,  0.293f, 0.0f,
     0.475f,  0.154f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.475f,  0.154f, 0.0f,
       0.5f,    0.0f, 0.0f,

       0.0f,    0.0f, 0.0f,
       0.5f,    0.0f, 0.0f,
     0.475f, -0.154f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.475f, -0.154f, 0.0f,
     0.404f, -0.293f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.404f, -0.293f, 0.0f,
     0.293f, -0.404f, 0.0f,

       0.0f,    0.0f, 0.0f,
     0.293f, -0.404f, 0.0f,
     0.154f, -0.475f, 0.0f,
    
       0.0f,    0.0f, 0.0f,
     0.154f, -0.475f, 0.0f,
       0.0f,   -0.5f, 0.0f,

       0.0f,    0.0f, 0.0f,
       0.0f,   -0.5f, 0.0f,
    -0.154f, -0.475f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.154f, -0.475f, 0.0f,
    -0.293f, -0.404f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.293f, -0.404f, 0.0f,
    -0.404f, -0.293f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.404f, -0.293f, 0.0f,
    -0.475f, -0.154f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.475f, -0.154f, 0.0f,
      -0.5f,    0.0f, 0.0f,

       0.0f,    0.0f, 0.0f,
      -0.5f,    0.0f, 0.0f,
    -0.475f,  0.154f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.475f,  0.154f, 0.0f,
    -0.404f,  0.293f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.404f,  0.293f, 0.0f,
    -0.293f,  0.404f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.293f,  0.404f, 0.0f,
    -0.154f,  0.475f, 0.0f,

       0.0f,    0.0f, 0.0f,
    -0.154f,  0.475f, 0.0f,
       0.0f,    0.5f, 0.0f  
};

void CollisionShape::update(float deltaTime) {

    // move keyboard
    if (react_to_keyboard->get()) {
        Camera::get_camera_singleton()->lock();

    } else {
        Camera::get_camera_singleton()->unlock();
    }
    // move forces
    if (react_to_forces->get()) {

    }

    // set aabb
    // TODO: set position and size based on size 
    aabb.set_position(aMath::Vec2(pos->get().x - 0.25, pos->get().y - 0.25));
    aabb.set_size(aMath::Vec2(0.5, 0.5));

    // calculate broad phase
    bool collides_broad_phase = PhysicsServer::get_singleton()->calculate_broad_phase_collision(broad_phase_collision_type->get(), this);
    // calculate narrow phase
    bool collides_narrow_phase = collides_broad_phase ? PhysicsServer::get_singleton()->calculate_narrow_phase_collision(narrow_phase_collision_type->get(), this) : false;
    // if collision occurs -> move object so that it roughly does not collide
    if (collides_narrow_phase) {
        // TODO: move object a little bit back
    }

}

void CollisionShape::render() {
    glUseProgram(shaderProgram);
    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "mvp_matrix");
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), color->get().x, color->get().y, color->get().z);

    aMath::Mat4 mvp;
    aMath::Mat4 model;

    model = aMath::mul(model, aMath::translate(pos->get()));
    model = aMath::mul(model, aMath::scale(0.5, 0.5, 0.5));

    mvp = aMath::mul(Camera::get_camera_singleton()->get_projection_matrix(), Camera::get_camera_singleton()->get_camera_matrix());
    mvp = aMath::mul(mvp, model);

    glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.values);
    glBindVertexArray(VAO_circle);
    glDrawArrays(GL_TRIANGLES, 0, 180);

    glBindVertexArray(0);
    glUseProgram(0);

    // draw AABB
    if (draw_aabb->get()) {
        DrawPen::get_drawpen_singleton()->set_stroke_size(0.25);
        DrawPen::get_drawpen_singleton()->set_draw_color(color->get());
        DrawPen::get_drawpen_singleton()->drawRect(aabb.get_position(), aabb.get_size());
    }
}

CollisionShape::CollisionShape(Property<aMath::Vec3>* p_pos, Property<aMath::Vec4>* p_color, Property<bool>* p_react_to_forces, Property<bool>* p_react_to_keyboard, Property<int>* p_shape, Property<int>* p_broad_phase_collision_type, Property<int>* p_narrow_phase_collision_type, Property<bool>* p_draw_aabb) :
    pos(p_pos), color(p_color), react_to_forces(p_react_to_forces), react_to_keyboard(p_react_to_keyboard), shape(p_shape), broad_phase_collision_type(p_broad_phase_collision_type), narrow_phase_collision_type(p_narrow_phase_collision_type), draw_aabb(p_draw_aabb) {

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
      ERROR("CollisionShape -- VERTEX: " << infoLog);
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
      ERROR("CollisionShape -- FRAG: " << infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      ERROR("CollisionShape -- LINK: " << infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &VBO_circle);

    glGenVertexArrays(1, &VAO_circle);
    glBindVertexArray(VAO_circle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_circle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CollisionShape::vertices_circle), CollisionShape::vertices_circle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // END OPENGL RENDERING

    PhysicsServer::get_singleton()->register_collision_shape(this);
}

CollisionShape::~CollisionShape() {
    PhysicsServer::get_singleton()->unregister_collision_shape(this);
}