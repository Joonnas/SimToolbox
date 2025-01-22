#ifndef ST_GUI_H
#define ST_GUI_H

#include <vector>

#include "ext/include/imgui/imgui.h"
#include "ext/include/imgui/imgui_impl_glfw.h"
#include "ext/include/imgui/imgui_impl_opengl3.h"

#include "utils/macros.h"
#include "gui/gui_elements/Gui_SimElement.h"

class Gui {
private:

    enum SimElements {
        NONE,
        PARTICLE_EMITTER,
        FORCE_FIELD, // TODO:
        BOIDS,
        RIGID_BODY, // TODO:
        COLLISSION_SHAPE, // TODO:
        FLUID_SIMULATOR // TODO:
    };

    SimElements deferred_element = SimElements::NONE;

    std::vector<Gui_SimElement*> ge;

    bool show_demo_window = true;
    bool show_new_element_window = false;

    void add_element_deferred(SimElements e);
    void apply_deferred_element();

    void renderMainMenuBar();
    void renderNewElementWindow();
    void renderViewport(ImVec2 vMin, ImVec2 vMax);

public:

    void update(float deltaTime);
    void render();

    Gui();
    ~Gui();
};

#endif