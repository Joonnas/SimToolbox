#include "Gui_CollisionShape.h"

int Gui_CollisionShape::ID = 0;


void Gui_CollisionShape::update(float deltaTime) {
    cs->update(deltaTime);
}

void Gui_CollisionShape::renderGui() {

    if (!ImGui::CollapsingHeader(name.c_str(), &is_visible))
        return;
    
    ImGui::PushID(id);
    if (ImGui::TreeNode("General")) {
        
        render_float_input_3("Position", true, position);
        ImGui::Text("Color button with Picker:");
        ImGui::ColorEdit4("##ColorEdit", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

        // reacts to forces (true, false) -> rigid body / static body
        ImGui::Checkbox("React to forces (e.g. Gravity)", &react_to_forces);
        react_to_forces.sync();
        // Keyboard input (true, false)
        ImGui::Checkbox("React to Keyboard (WASD)", &react_to_keyboard);
        react_to_keyboard.sync();

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Body")) {

        // shape (circle, rect, pentagon, octagon, world-boundary)
        ImGui::SeparatorText("Shape");
        ImGui::Combo("##Shape", &shape, "Circle\0Rectangle\0Pentagon\0Octagon\0World Boundary");
        shape.sync();
        if (shape == CollisionShape::Shapes::SHAPE_CIRCLE) {

        } else if (shape == CollisionShape::Shapes::SHAPE_RECTANGLE) {

        } else if (shape == CollisionShape::Shapes::SHAPE_PENTAGON) {

        } else if (shape == CollisionShape::Shapes::SHAPE_OCTAGON) {

        } else if (shape == CollisionShape::Shapes::SHAPE_WORLD_BOUNDARY) {
            
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Collision")) {

        // broad phase collision-type (Sphere, AABB, OBB, ...)
        ImGui::Text("Broad Phase Collision");
        ImGui::Combo("##Broad Phase Collision", &broad_phase_collision_type, "Sphere\0AABB\0OBB\0kDOP");
        broad_phase_collision_type.sync();
        // narrow phase collision-type (GJK, Voronoid-regions)
        ImGui::Text("Narrow Phase Collision");
        ImGui::Combo("##Narrow Phase Collision", &narrow_phase_collision_type, "Voronoi Regions\0GJK");
        narrow_phase_collision_type.sync();

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Custom scripts")) {
        // custom broad phase collision
        ImGui::Checkbox("Custom Broad Phase Method", &use_custom_broad_phase);
        if (use_custom_broad_phase) {
            ImGui::Text("Code Editor Broad Phase");
            if (code_string_broad_phase.empty())
                code_string_broad_phase.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor broad phase", &code_string_broad_phase, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }
        // custom narrow phase collision
        ImGui::Checkbox("Custom Narrow Phase Method", &use_custom_narrow_phase);
        if (use_custom_narrow_phase) {
            ImGui::Text("Code Editor Narrow Phase");
            if (code_string_narrow_phase.empty())
                code_string_narrow_phase.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor narrow phase", &code_string_narrow_phase, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Debug")) {

        ImGui::TreePop();
    }
    ImGui::PopID();


}

void Gui_CollisionShape::render() {
    cs->render();
}

Gui_CollisionShape::Gui_CollisionShape() {
    name = "CollisionShape" + std::to_string(Gui_CollisionShape::ID);
    id = Gui_CollisionShape::ID++;

    cs = new CollisionShape(position.getPointer(), color.getPointer(), react_to_forces.getPointer(), react_to_keyboard.getPointer(), shape.getPointer(), broad_phase_collision_type.getPointer(), narrow_phase_collision_type.getPointer());
}

// TODO: Rigid body will be a collisionshape that reacts differently (in a correct way) to forces