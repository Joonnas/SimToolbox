#ifndef ST_COLLISION_SHAPE_H
#define ST_COLLISION_SHAPE_H

#include <string>

#include "ext/include/imgui/imgui.h"
#include "Gui_SimElement.h"

#include "physics/CollisionShape.h"

class Gui_CollisionShape : public Gui_SimElement {
private:
    static int ID;

private:
    int id;

    Property<bool> react_to_forces = false;
    Property<bool> react_to_keyboard = false;

    Property<int> shape = 0; // 0 = circle

    Property<int> broad_phase_collision_type = 0;
    Property<int> narrow_phase_collision_type = 0;

    // Radius properties
    // Property<float> circle_radius = 1.0;

    // // AABB properties
    // Property<aMath::Vec2> extents = aMath::Vec2(1.0, 1.0);

    Property<bool> use_custom_broad_phase = false;
    Property<bool> use_custom_narrow_phase = false;

    ImVector<char> code_string_broad_phase;
    ImVector<char> code_string_narrow_phase;

    CollisionShape* cs;

public:
    void update(float deltaTime);
    void renderGui();
    void render();

    Gui_CollisionShape();
    ~Gui_CollisionShape() {
        delete cs;
    }

};

#endif