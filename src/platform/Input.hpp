#pragma once
#include "platform/Window.hpp"

namespace platform {

    struct MouseState {
        double x = 0.0;
        double y = 0.0;
        double deltaX = 0.0;       
        double deltaY = 0.0;
        double scrollDelta = 0.0;   
        bool leftPressed = false;
        bool rightPressed = false;
        bool middlePressed = false;
        bool overUI = false;
    };

    class Input {
    public:
        static bool isKeyPressed(const Window& window, int glfwKey);
        static MouseState& mouse();
        static void resetFrameDeltas();
    };

}