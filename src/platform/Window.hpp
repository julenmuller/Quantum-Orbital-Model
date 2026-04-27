// ============================================================
// Window.hpp
// ------------------------------------------------------------
// Wrapper sobre GLFW. Encapsula:
//   - Criação da janela
//   - Inicialização do contexto OpenGL
//   - Gestão de tamanho/eventos básicos
//
// O resto do código não toca em GLFW direto — fala com Window.
// Isso facilita trocar de backend (SDL, etc.) no futuro.
// ============================================================

#pragma once

#include <string>

struct GLFWwindow;

namespace platform {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Não-copiável (gerencia recurso bruto)
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Verifica se o usuário pediu pra fechar (X, Alt+F4, etc.)
    bool shouldClose() const;

    // Sinaliza pra fechar (chamado pelo ESC, por exemplo)
    void requestClose();

    // Troca os buffers (mostra o frame desenhado)
    void swapBuffers();

    // Processa eventos pendentes (teclado, mouse, redimensionar)
    void pollEvents();

    int width() const { return width_; }
    int height() const { return height_; }
    float aspectRatio() const {
        return height_ > 0 ? static_cast<float>(width_) / height_ : 1.0f;
    }

    GLFWwindow* nativeHandle() const { return handle_; }

private:
    static void framebufferSizeCallback(GLFWwindow* w, int width, int height);

    GLFWwindow* handle_ = nullptr;
    int width_;
    int height_;
};

} // namespace platform
