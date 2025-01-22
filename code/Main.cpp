// #define GLEW_STATIC

#include <iostream>
#include <string>

#include "ext/include/GL/glew.h"
#include "ext/include/GLFW/glfw3.h"

#include "gui/Window.h"
#include "utils/macros.h"


int main() {

    int frame_counter = 0;
    float secondsCounter = 0.0f;

    Window w(1280, 720);

    float lastTime = glfwGetTime();
    while(!w.shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        

        frame_counter++;
        secondsCounter += deltaTime;

        if (secondsCounter >= 1.0) {
            // INFO("FPS: " + std::to_string(frame_counter));
            secondsCounter -= 1.0;
            frame_counter = 0;
        }

        w.update(deltaTime);
        w.render();

        lastTime = currentTime;

        // std::cout << "Test counter: " << GetCounter() << std::endl;
    }

    return 0;

}