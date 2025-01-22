#include "Gui_ForceField.h"

int Gui_ForceField::ID = 0;

void Gui_ForceField::update(float deltaTime) {

}

void Gui_ForceField::renderGui() {
    if (!ImGui::CollapsingHeader(name.c_str(), &is_visible))
        return;
}

void Gui_ForceField::render() {

}

Gui_ForceField::Gui_ForceField() {
    name = "ForceField" + std::to_string(Gui_ForceField::ID);
    id = Gui_ForceField::ID++;
}
