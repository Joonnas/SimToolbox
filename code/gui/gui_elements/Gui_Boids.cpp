#include "Gui_Boids.h"

int Gui_Boids::ID = 0;

void Gui_Boids::update(float deltaTime) {
    b->update(deltaTime);
}

void Gui_Boids::renderGui() {
    if(!ImGui::CollapsingHeader(name.c_str(), &is_visible))
        return;
    
    ImGui::PushID(id);
    if (ImGui::TreeNode("General")) {
        render_float_input_3("Position", true, position);
        ImGui::InputInt("Amount", &boids_amount);
        boids_amount.sync();
        render_float_input_2("Boundary", true, custom_boundary_rect);

        ImGui::Spacing();
        ImGui::SeparatorText("Boid Perception Field");
        ImGui::Combo("##Boid Perception Field", &boid_perception_field, "Centered Rectangle\0Following Rectangle\0Circle Arc");
        boid_perception_field.sync();
        Boids::BoidPerceptionField bpf = static_cast<Boids::BoidPerceptionField>(boid_perception_field.get());
        if (bpf == Boids::BoidPerceptionField::BOID_PERCEPTION_CENTERED_RECT || bpf == Boids::BoidPerceptionField::BOID_PERCEPTION_FOLLOWING_RECT) {
            render_float_input_2("Perception Field Size", true, bpf_rect_size);
        } else if (bpf == Boids::BoidPerceptionField::BOID_PERCEPTION_CIRCULAR_ARC) {
            ImGui::Text("Perception Field Radius");
            ImGui::SliderFloat("##perception field radius", &bpf_circ_radius, 0.0f, 1.0f, "%.2f");
            bpf_circ_radius.sync();

            ImGui::Text("Perception Field Arc Angle");
            ImGui::SliderFloat("##perception field arc angle", &bpf_circ_arc_angle, 0.0f, 360.0f, "%.2f");
            bpf_circ_arc_angle.sync();
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Behaviour")) {
        ImGui::Text("Max Speed");
        ImGui::SliderFloat("##max_speed", &max_speed, 0.0f, 5.0f, "%.2f");
        max_speed.sync();

        ImGui::Text("Seperation");
        ImGui::SliderFloat("##seperation", &seperation, 0.0f, 5.0f, "%.2f");
        seperation.sync();

        ImGui::Text("Alignment");
        ImGui::SliderFloat("##alignment", &alignment, 0.0f, 5.0f, "%.2f");
        alignment.sync();

        ImGui::Text("Cohesion");
        ImGui::SliderFloat("##cohesion", &cohesion, 0.0f, 5.0f, "%.2f");
        cohesion.sync();

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Custom scripts")) {

        ImGui::Checkbox("Custom update method", &use_custom_update);
        if (use_custom_update) {
            ImGui::Text("Code Editor Update");
            if (code_string_update.empty())
                code_string_update.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor update", &code_string_update, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::Checkbox("Custom seperation method", &use_custom_seperation);
        if (use_custom_seperation) {
            ImGui::Text("Code Editor Seperation");
            if (code_string_seperation.empty())
                code_string_seperation.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor seperation", &code_string_seperation, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::Checkbox("Custom alignment method", &use_custom_alignment);
        if (use_custom_alignment) {
            ImGui::Text("Code Editor Alignment");
            if (code_string_alignment.empty())
                code_string_alignment.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor alignment", &code_string_alignment, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::Checkbox("Custom cohesion method", &use_custom_cohesion);
        if (use_custom_cohesion) {
            ImGui::Text("Code Editor Cohesion");
            if (code_string_cohesion.empty())
                code_string_cohesion.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor cohesion", &code_string_cohesion, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::Checkbox("Custom additional function", &use_custom_func);
        if (use_custom_func) {
            ImGui::Text("Code Editor additional function");
            if (code_string_func.empty())
                code_string_func.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor additional function", &code_string_func, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Debug")) {
        ImGui::Checkbox("Draw Quadtree", &draw_quadtree);
        draw_quadtree.sync();
        ImGui::Checkbox("Draw Border", &draw_border);
        draw_border.sync();
        ImGui::Checkbox("Draw Visual Field", &draw_boid_visual_field);
        draw_boid_visual_field.sync();
        ImGui::Checkbox("Draw Velocities", &draw_boid_velocity);
        draw_boid_velocity.sync();

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void Gui_Boids::render() {
    b->render();
}

Gui_Boids::Gui_Boids() {
    name = "Boids" + std::to_string(Gui_Boids::ID);
    id = Gui_Boids::ID++;

    b = new Boids(position.getPointer(), boids_amount.getPointer(), custom_boundary_rect.getPointer(), boid_perception_field.getPointer(), bpf_rect_size.getPointer(), bpf_circ_radius.getPointer(), bpf_circ_arc_angle.getPointer(), max_speed.getPointer(), seperation.getPointer(), alignment.getPointer(), cohesion.getPointer(), draw_quadtree.getPointer(), draw_border.getPointer(), draw_boid_visual_field.getPointer(), draw_boid_velocity.getPointer());
}