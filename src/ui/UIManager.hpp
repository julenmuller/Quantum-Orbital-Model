// ============================================================
// UIManager.hpp
// ------------------------------------------------------------
// Gerencia o ciclo de vida do Dear ImGui:
//   - Inicialização (vincula com GLFW + OpenGL3)
//   - beginFrame/endFrame por frame
//   - Shutdown
// ============================================================

#pragma once

#include "platform/Window.hpp"

namespace ui {

class UIManager {
public:
    UIManager();
    ~UIManager();

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    bool initialize(platform::Window& window);
    void shutdown();

    void beginFrame();
    void endFrame();

    bool wantsCaptureMouse() const;
    bool wantsCaptureKeyboard() const;

private:
    bool initialized_ = false;
};

} // namespace ui
