// ============================================================
// Input.cpp
// ============================================================

#include "platform/Input.hpp"

#include <GLFW/glfw3.h>

namespace platform {

bool Input::isKeyPressed(const Window& window, int glfwKey) {
    return glfwGetKey(window.nativeHandle(), glfwKey) == GLFW_PRESS;
}

} // namespace platform
