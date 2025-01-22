#include "draw.h"

#include "ext/include/GL/glew.h"

#include "gui/Camera.h"

float DrawPen::vertices_rect[18] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
};

bool DrawPen::singleton_created = false;
DrawPen* DrawPen::drawpen_singleton = nullptr;


void DrawPen::_draw(aMath::Mat4 mvp, aMath::Color_RGBA col) {
    glUseProgram(shaderProgram);
    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "mvp_matrix");
    glUniform4f(glGetUniformLocation(shaderProgram, "color"), col.x, col.y, col.z, col.w);

    glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.values);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);
}

void DrawPen::drawLine(aMath::Point2 a, aMath::Point2 b) {
    // calculate direction vector
    aMath::Vec2 direction2 = b - a;
    aMath::Vec3 direction3 = aMath::Vec3(direction2.x, direction2.y, 0.0);
    aMath::Vec3 dir_normal = direction3;
    dir_normal.normalize();
    // calculate rotation based on direction (see boids for that)
    float rot = asin(abs(dir_normal.y) / dir_normal.length());
    float rotation = 0.0f;
    if (direction3.x >= 0.0f && direction3.y >= 0.0f) {
        rotation = rot;
    } else if (direction3.x < 0.0f && direction3.y >= 0.0f) {
        rotation = aMath::PI - rot;
    } else if (direction3.x >= 0.0f && direction3.y < 0.0f) {
        rotation = aMath::TAU - rot;
    } else {
        rotation = aMath::PI + rot;
    }

    aMath::Mat4 mvp;
    aMath::Mat4 model;

    float length = direction3.length();
    aMath::Vec3 pos = aMath::Vec3(a.x, a.y, 0.0) + dir_normal * (length / 2.0);

    model = aMath::mul(model, aMath::translate(pos));
    model = aMath::mul(model, aMath::rotateZ(rotation));
    model = aMath::mul(model, aMath::scale(length, stroke_size / 100.0, 1.0));

    mvp = aMath::mul(Camera::get_camera_singleton()->get_projection_matrix(), Camera::get_camera_singleton()->get_camera_matrix());
    mvp = aMath::mul(mvp, model);

    _draw(mvp, draw_color);
}

void DrawPen::drawRect(aMath::Point2 upper_left, aMath::Vec2 dim) {
    drawLine(upper_left, aMath::Point2(upper_left.x + dim.x, upper_left.y));
    drawLine(aMath::Point2(upper_left.x + dim.x, upper_left.y), upper_left + dim);
    drawLine(upper_left + dim, aMath::Point2(upper_left.x, upper_left.y + dim.y));
    drawLine(aMath::Point2(upper_left.x, upper_left.y + dim.y), upper_left);
}

void DrawPen::fillRect(aMath::Point2 upper_left, aMath::Vec2 dim, bool draw_border) {
    aMath::Mat4 mvp_base;
    aMath::Mat4 mvp_full;
    aMath::Mat4 model;
    // ( x + (0.5 * scaleX), y - (0.5 * scaleY) ) / dim
    aMath::Vec3 pos = aMath::Vec3(upper_left.x + (0.5 * dim.x), upper_left.y + (0.5 * dim.y), 0.0);

    mvp_base = aMath::mul(Camera::get_camera_singleton()->get_projection_matrix(), Camera::get_camera_singleton()->get_camera_matrix());

    model = aMath::mul(model, aMath::translate(pos));
    model = aMath::mul(model, aMath::scale(aMath::Vec3(dim.x, dim.y, 1.0)));

    mvp_full = aMath::mul(mvp_base, model);
    _draw(mvp_full, fill_color);

    if (draw_border) drawRect(upper_left, dim);
}

DrawPen::DrawPen() {

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
        ERROR("DrawPen -- VERTEX: " << infoLog);
    }

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 color;\n"
        "void main() {\n"
        "   FragColor = color;\n"
        "}\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        ERROR("DrawPen -- FRAG: " << infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        ERROR("DrawPen -- LINK: " << infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DrawPen::vertices_rect), DrawPen::vertices_rect, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // END OPENGL RENDERING

}