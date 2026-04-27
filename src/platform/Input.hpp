// ============================================================
// Input.hpp
// ------------------------------------------------------------
// Helpers de input — checagens simples de teclado/mouse.
// ============================================================

#pragma once

#include "platform/Window.hpp"

namespace platform {

class Input {
public:
    // Checa se uma tecla está pressionada agora (códigos GLFW).
    static bool isKeyPressed(const Window& window, int glfwKey);
};

} // namespace platform
