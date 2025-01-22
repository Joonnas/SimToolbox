#ifndef ST_GUI_BOIDS_H
#define ST_GUI_BOIDS_H

#include <string>

#include "ext/include/imgui/imgui.h"
#include "Gui_SimElement.h"

#include "physics/Boids.h"

class Gui_Boids : public Gui_SimElement {
private:
    static int ID;

private:
    int id;

    // General
    Property<int> boids_amount = 100;
    Property<aMath::Vec2> custom_boundary_rect = aMath::Vec2(0.5, 0.5);
    Property<int> boid_perception_field = 0;
    Property<aMath::Vec2> bpf_rect_size = aMath::Vec2(0.2, 0.2);
    Property<float> bpf_circ_radius = 0.1;
    Property<float> bpf_circ_arc_angle = 270.0;

    // Behaviour
    Property<float> max_speed = 0.5;
    Property<float> seperation = 1.5;
    Property<float> alignment = 3.5;
    Property<float> cohesion = 2.2;

    // Custom scripts
    Property<bool> use_custom_update = false;
    Property<bool> use_custom_seperation = false;
    Property<bool> use_custom_alignment = false;
    Property<bool> use_custom_cohesion = false;
    Property<bool> use_custom_func = false;

    ImVector<char> code_string_update;
    ImVector<char> code_string_seperation;
    ImVector<char> code_string_alignment;
    ImVector<char> code_string_cohesion;
    ImVector<char> code_string_func;

    // Debug
    Property<bool> draw_quadtree = false;
    Property<bool> draw_border = false;
    Property<bool> draw_boid_visual_field = false;
    Property<bool> draw_boid_velocity = false;

    Boids* b;

public:

    void update(float deltaTime);
    void renderGui();
    void render();

    Gui_Boids();
    ~Gui_Boids() {
        delete b;
    };
};

#endif