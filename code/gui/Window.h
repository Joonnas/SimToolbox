#ifndef ST_WINDOW_H
#define ST_WINDOW_H

#define GLEW_STATIC

#include "ext/include/imgui/imgui.h"
#include "ext/include/imgui/imgui_impl_glfw.h"
#include "ext/include/imgui/imgui_impl_opengl3.h"
#include "ext/include/GL/glew.h"
#include "ext/include/GLFW/glfw3.h"

#include "utils/macros.h"
#include "gui/Gui.h"


class Window {
private:
    GLFWwindow* window;
    int width, height;
    const char* name;
    const char* title;

    bool vsync = true;

    Gui gui; 

public:

    void update(float deltaTime);

    bool shouldClose();
    void render(); 

    bool get_vsync() const { return vsync; };
    void set_vsync(bool p_vsync) { vsync = p_vsync; };

    Window();
    Window(int w, int h);
    ~Window();

};


#endif