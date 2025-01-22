#include "Gui_ParticleEmitter.h"

#include "Utils/macros.h"


int Gui_ParticleEmitter::ID = 0;

void Gui_ParticleEmitter::update(float deltaTime) {
    pe->update(deltaTime);
}

void Gui_ParticleEmitter::renderGui() {

    if (!ImGui::CollapsingHeader(name.c_str(), &is_visible))
        return;

    ImGui::PushID(id);
    if (ImGui::TreeNode("General")) {
        render_float_input_3("Position", true, position);

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Emitter")) {
        ImGui::Checkbox("Emitting", &emitting);
        emitting.sync();
        ImGui::Checkbox("One shot", &oneshot);
        oneshot.sync();
        ImGui::InputInt("Amount", &particles_amount);
        particles_amount.sync();

        ImGui::Spacing();
        ImGui::SeparatorText("Emission Shape");
       
        ImGui::Combo("##Emission Shape", &emission_shape, "Point\0Sphere\0Rect\0\0");
        emission_shape.sync();
        ParticleEmitter::EmissionShapes es = static_cast<ParticleEmitter::EmissionShapes>(emission_shape.get());
        if (es == ParticleEmitter::EmissionShapes::SHAPE_SPHERE) {
            ImGui::Text("Sphere Radius");
            ImGui::SliderFloat("##sphere radius", &sphere_radius, 0.0f, 1.0f, "%.2f");
            sphere_radius.sync();
        } else if (es == ParticleEmitter::EmissionShapes::SHAPE_RECT) {
            render_float_input_2("Rect extents", true, rect_extents);
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Emission Behaviour");
        
        ImGui::Text("Explosiveness");
        ImGui::SliderFloat("##explosiveness", &explosiveness, 0.0f, 1.0f, "%.1f");
        explosiveness.sync();

        ImGui::Spacing();
        ImGui::SeparatorText("Initial conditions");

        render_float_input_3("Initial Velocity", true, initial_velocity);
        ImGui::SliderFloat("Spread", &spread, 0.0f, 1.0f, "%.2f");
        spread.sync();
        render_float_input_3("Constant Acceleration", true, constant_acceleration);
        

        ImGui::TreePop();
    }
    ImGui::PopID();

    ImGui::PushID(id);
    if (ImGui::TreeNode("Particles")) {
        ImGui::Text("Particle Lifetime");
        ImGui::SliderFloat("##particle lifetime", &particle_lifetime, 0.1f, 600.0f, "%.1f");
        particle_lifetime.sync();
        ImGui::Text("Lifetime Randomness");
        ImGui::SliderFloat("##lifetime randomness", &lifetime_randomness, 0.0f, 1.0f, "%.1f");
        lifetime_randomness.sync();

        ImGui::Checkbox("Custom update method", &use_custom_script);
        if (use_custom_script) {
            ImGui::Text("Code Editor");
            if (code_string.empty())
                code_string.push_back(0);
            Funcs::CustomInputTextMultiline("##Code editor", &code_string, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8));
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
    
}

void Gui_ParticleEmitter::render() {
    pe->render();
}

Gui_ParticleEmitter::Gui_ParticleEmitter() {
    name = "ParticleEmitter" + std::to_string(Gui_ParticleEmitter::ID);
    id = Gui_ParticleEmitter::ID++;

    pe = new ParticleEmitter(position.getPointer(), emitting.getPointer(), oneshot.getPointer(), particles_amount.getPointer(), emission_shape.getPointer(), sphere_radius.getPointer(), rect_extents.getPointer(), explosiveness.getPointer(), initial_velocity.getPointer(), spread.getPointer(), constant_acceleration.getPointer(), particle_lifetime.getPointer(), lifetime_randomness.getPointer());
}