#include "Window.h"

#include "Camera.h"
#include "Utils/draw.h"
#include "physics/PhysicsServer.h"

void Window::update(float deltaTime) {
    glfwPollEvents();

    Camera::get_camera_singleton()->update(deltaTime);
    gui.update(deltaTime);
}

void Window::render() {
    
    // DO stuff
    
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    gui.render();

    if (vsync)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    
    glfwSwapBuffers(window);
}



bool Window::shouldClose() {
    if (window) return glfwWindowShouldClose(window);
    else return true;
}


Window::Window() {
    Window(1280, 720);
}

Window::Window(int w, int h) {
    width = w;
    height = h;
    name = "SimToolbox";
    title = "Simulation Toolbox";

    if (!glfwInit()) {
        ERROR("Could not initialize GLFW!");
        throw -1;
    }

    window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!window) {
        ERROR("Could not create window!");
        throw -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        ERROR("Could not initialize GLEW");
        throw -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    Camera2D::create_singleton();
    Camera::get_camera_singleton()->change_projection_matrix(aMath::Vec4(-1.0, 1.0, -1.0, 1.0));

    DrawPen::create_singleton();
    PhysicsServer::create_singleton();

}


Window::~Window() {

    PhysicsServer::delete_singleton();
    DrawPen::delete_singleton();
    Camera::delete_singleton();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
