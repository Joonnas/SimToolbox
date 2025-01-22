#include "Gui.h"

#include "gui/Camera.h"
#include "gui/gui_elements/Gui_ParticleEmitter.h"
#include "gui/gui_elements/Gui_ForceField.h"
#include "gui/gui_elements/Gui_Boids.h"
#include "gui/gui_elements/Gui_CollisionShape.h"

void Gui::add_element_deferred(SimElements e) {
    deferred_element = e;
}
void Gui::apply_deferred_element() {
    if (deferred_element == SimElements::NONE) {return;}
    if (deferred_element == SimElements::PARTICLE_EMITTER) {    
        ge.push_back(new Gui_ParticleEmitter());
    } else if(deferred_element == SimElements::FORCE_FIELD) {
        ge.push_back(new Gui_ForceField());
    } else if(deferred_element == SimElements::BOIDS) {
        ge.push_back(new Gui_Boids());
    } else if(deferred_element == SimElements::COLLISSION_SHAPE) {
        ge.push_back(new Gui_CollisionShape());
    } // else if (deferred_element == ...) {}

    deferred_element = SimElements::NONE;
}


void Gui::renderNewElementWindow() {
    ImGui::Begin("Add Element", &show_new_element_window);

    if (ImGui::Button("ParticleEmitter")) {
        add_element_deferred(SimElements::PARTICLE_EMITTER);
        show_new_element_window = false;
    }
    if (ImGui::Button("ForceField")) {
        add_element_deferred(SimElements::FORCE_FIELD);
        show_new_element_window = false;
    }
    if (ImGui::Button("Boids")) {
        add_element_deferred(SimElements::BOIDS);
        show_new_element_window = false;
    }
    if (ImGui::Button("CollisionShape")) {
        add_element_deferred(SimElements::COLLISSION_SHAPE);
        show_new_element_window = false;
    }

    ImGui::End();
}

void Gui::renderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Elements")) {
            if (ImGui::MenuItem("Add Element", "CTRL+A")) {show_new_element_window = true;}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples")) {
            if (ImGui::BeginMenu("Particles")) {
                if (ImGui::MenuItem("Simple particle motion")) {DEBUG("Load example: Simple particle motion")};
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("RigidBodies")) {
                if (ImGui::MenuItem("Torque")) {DEBUG("Load example: Torque");}
                if (ImGui::MenuItem("Collision of two balls")) {DEBUG("Load example: Collision of two balls")}
                if (ImGui::MenuItem("Collision of two complex objects")) {DEBUG("Load example: Collision of two complex objects");}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Fluid Simulation")) {
                if (ImGui::MenuItem("Shallow Water")) {DEBUG("Load example: Shallow water");}
                if (ImGui::MenuItem("SPH")) {DEBUG("Load example: SPH");}
                if (ImGui::MenuItem("Grids")) {DEBUG("Load example: Grids");}
                if (ImGui::MenuItem("Oceans")) {DEBUG("Load example: Oceans")}
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Exercises")) {
            if (ImGui::MenuItem("Exercise 1: Single particles")) {DEBUG("Load Exercise 1: Single particles")}
            if (ImGui::MenuItem("Exercise 2: Multiple particles")) {DEBUG("Load Exercise 2: Multiple particles")}
            if (ImGui::MenuItem("Exercise 3: Rigidbody motion")) {DEBUG("Load Exercise 3: Rigidbody motion")}
            if (ImGui::MenuItem("Exercise 4: Rigidbody collision AABB")) {DEBUG("Load Exercise 4: Rigidbody collision AABB")}
            if (ImGui::MenuItem("Exercise 5: Rigidbody collision GJK")) {DEBUG("Load Exercise 5: Rigidbody collision GJK")}
            if (ImGui::MenuItem("Exercise 6: Rigidbody collision response")) {DEBUG("Load Exercise 6: Rigidbody collision response")}
            if (ImGui::MenuItem("Exercise 7: Fluid Sim Wave equation")) {DEBUG("Load Exercise 7: Fluid Sim Wave equation")}
            if (ImGui::MenuItem("Exercise 8: Fluid Sim Shallow Water equations")) {DEBUG("Load Exercise 8: Fluid Sim Shallow Water equations")}
            if (ImGui::MenuItem("Exercise 9: Fluid Sim SPH")) {DEBUG("Load Exercise 9: Fluid Sim SPH")}


            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Gui::renderViewport(ImVec2 vMin, ImVec2 vMax) {
    // render the viewport
    int w, h;
    w = vMax.x - vMin.x;
    h = vMax.y - vMin.y;
    glViewport(vMin.x, vMin.y, w, h);
    Camera::get_camera_singleton()->set_camera_size(aMath::Vec3(1.0 / (w / Camera::BASE_WIDTH), 1.0 / (h / Camera::BASE_HEIGHT), 1.0));
    Camera::get_camera_singleton()->set_viewport(aMath::Vec2(vMin.x, vMin.y), aMath::Vec2(w, h));
    for (int i = 0; i < ge.size(); i++) {
        ge[i]->render();
    }
}

void Gui::update(float deltaTime) {
    std::vector<int> to_erase;
    for (int i = 0; i < ge.size(); i++) {
        ge[i]->update(deltaTime);
        if (ge[i]->should_delete()) {
            to_erase.push_back(i);
        }
    }

    // delete all elements that are marked to be deleted
    for (int i = to_erase.size() - 1; i >= 0; i--) {
        Gui_SimElement* g = ge[to_erase[i]];
        INFO("DELETE " + g->get_name());
        ge.erase(ge.begin() + to_erase[i]);
        delete g;
    }
}

void Gui::render() {

    ImVec2 vMin;
    ImVec2 vMax;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    renderMainMenuBar();

    if (show_new_element_window) { renderNewElementWindow(); }

    // ImGuiWindowClass window_class;
    // window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    // ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin("SceneTree");
    for (int i = 0; i < ge.size(); i++) {
        ge[i]->renderGui();
    }
    ImGui::End();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
    // ImGui::PushStyleColor()
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;
    ImGui::Begin("Viewport", (bool*) true, window_flags);

    vMin = ImGui::GetWindowContentRegionMin();
    vMax = ImGui::GetWindowContentRegionMax();
    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;
    // ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 0, 0, 255));

    ImGui::End();
    ImGui::PopStyleColor();

    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // TODO: Render the viewport to an FBO -> texture and let ImGui render that Image -> Viewport can be in between of layers
    renderViewport(vMin, vMax);

    apply_deferred_element();
}

Gui::Gui() {

}

Gui::~Gui() {
    for (int i = 0; i < ge.size(); i++) {
        delete ge[i];
    }
}