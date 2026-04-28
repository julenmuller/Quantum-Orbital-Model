#include "platform/Input.hpp"
#include <GLFW/glfw3.h>

namespace platform {

    namespace {
        MouseState g_mouseState;
    }

    bool Input::isKeyPressed(const Window& window, int glfwKey) {
        return glfwGetKey(window.nativeHandle(), glfwKey) == GLFW_PRESS;
    }

    MouseState& Input::mouse() {
        return g_mouseState;
    }

    void Input::resetFrameDeltas() {
        g_mouseState.deltaX = 0.0;
        g_mouseState.deltaY = 0.0;
        g_mouseState.scrollDelta = 0.0;
    }

}