#ifndef ST_DRAW_H
#define ST_DRAW_H

#include "ext/include/aMath/a_math.h"

#include "Utils/macros.h"

class DrawPen {
private:
    static float vertices_rect[18];
    // static float vertices_triangle [9];

    static bool singleton_created;
    static DrawPen* drawpen_singleton;

public:

    static void create_singleton() { if (!singleton_created) {drawpen_singleton = new DrawPen(); singleton_created = true;}};
    static void delete_singleton() { if (singleton_created) {delete drawpen_singleton; singleton_created = false; }};
    static DrawPen* get_drawpen_singleton() { return drawpen_singleton; };

private:
    
    unsigned int VBO;
    unsigned int VAO;
    unsigned int shaderProgram;

    aMath::Color_RGBA draw_color = aMath::Color_RGBA(1.0, 1.0, 1.0, 1.0);
    aMath::Color_RGBA fill_color = aMath::Color_RGBA(1.0, 1.0, 1.0, 1.0);

    float stroke_size = 1.0;

private:

    void _draw(aMath::Mat4 mvp, aMath::Color_RGBA col);

public:

    void set_draw_color(aMath::Color_RGBA p_draw_color) { draw_color = p_draw_color; };
    void set_fill_color(aMath::Color_RGBA p_fill_color) { fill_color = p_fill_color; };
    void set_stroke_size(float p_stroke_size) { stroke_size = p_stroke_size; };

    void drawLine(aMath::Point2 a, aMath::Point2 b);
    void drawRect(aMath::Point2 upper_left, aMath::Vec2 dim);
    void fillRect(aMath::Point2 upper_left, aMath::Vec2 dim, bool draw_border);

    DrawPen();
    ~DrawPen() {};

};

#endif