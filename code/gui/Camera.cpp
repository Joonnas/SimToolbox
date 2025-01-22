#include "Camera.h"

#include "ext/include/imgui/imgui.h"

const float Camera::BASE_WIDTH = 1280.0;
const float Camera::BASE_HEIGHT = 1280.0;
const float Camera::BASE_ASPECT_RATIO = 1280.0/720.0;

Camera* Camera::cam_singleton = nullptr;
bool Camera::singleton_created = false;


void Camera2D::update(float deltaTime) {

    // if Mouse point is not in viewport: return
    ImVec2 mouse_pos = ImGui::GetMousePos();
    if (!(mouse_pos.x > vpos.x && mouse_pos.x < (vpos.x + vsize.x) && mouse_pos.y > vpos.y && mouse_pos.y < (vpos.y + vsize.y))) {
        return;
    }
    if (is_locked) {
        return;
    }

    // Camera movement
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);

        // Left
        if (ImGui::IsKeyDown(ImGuiKey_A)) {
            set_camera_position(position - (aMath::Vec3(-0.5, 0.0, 0.0) * deltaTime));

        // RIGTH
        } else if (ImGui::IsKeyDown(ImGuiKey_D)) {
            set_camera_position(position - (aMath::Vec3(0.5, 0.0, 0.0) * deltaTime));

        // UP 
        } else if (ImGui::IsKeyDown(ImGuiKey_W)) {
            set_camera_position(position - (aMath::Vec3(0.0, -0.5, 0.0) * deltaTime));
        
        // DOWN
        } else if (ImGui::IsKeyDown(ImGuiKey_S)) {
            set_camera_position(position - (aMath::Vec3(0.0, 0.5, 0.0) * deltaTime));
            
        }

    }

}