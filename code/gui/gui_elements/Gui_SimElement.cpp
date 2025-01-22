#include "Gui_SimElement.h"


void Gui_SimElement::render_float_input_2(const char* title, bool show_xy, Property<aMath::Vec2> &value) {
    ImGui::PushID(title);
    ImGui::Text(title);
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
    ImGui::DragFloat((show_xy) ? "X" : "##X", &(&value)->x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    ImGui::SameLine();
    ImGui::DragFloat((show_xy) ? "Y" : "##Y", &(&value)->y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    value.sync();
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void Gui_SimElement::render_float_input_3(const char* title, bool show_xyz, Property<aMath::Vec3> &value) {
    ImGui::PushID(title);
    ImGui::Text(title);
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
    ImGui::DragFloat((show_xyz) ? "X" : "##X", &(&value)->x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    ImGui::SameLine();
    ImGui::DragFloat((show_xyz) ? "Y" : "##Y", &(&value)->y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    ImGui::SameLine();
    ImGui::DragFloat((show_xyz) ? "Z" : "##Z", &(&value)->z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    value.sync();
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void Gui_SimElement::render_float_input_4(const char* title, bool show_xyzw, Property<aMath::Vec4> &value) {
    ImGui::PushID(title);
    ImGui::Text(title);
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
    ImGui::DragFloat((show_xyzw) ? "X" : "##X", &(&value)->x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    ImGui::SameLine();
    ImGui::DragFloat((show_xyzw) ? "Y" : "##Y", &(&value)->y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    ImGui::SameLine();
    ImGui::DragFloat((show_xyzw) ? "Z" : "##Z", &(&value)->z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    ImGui::SameLine();
    ImGui::DragFloat((show_xyzw) ? "W" : "##W", &(&value)->w, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f");
    value.sync();
    ImGui::PopItemWidth();
    ImGui::PopID();
}