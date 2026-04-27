// ============================================================
// Window.cpp - VERSÃO COM FALLBACK PARA DRIVERS ANTIGOS
// ============================================================

#include "platform/Window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace platform {

    namespace {
        int g_windowCount = 0;

        void glfwErrorCallback(int code, const char* description) {
            std::cerr << "[GLFW Error " << code << "] " << description << std::endl;
            std::cerr.flush();
        }

        GLFWwindow* tryCreateWindow(int width, int height, const std::string& title,
            int major, int minor, bool core) {
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
            if (core) {
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            }
            else {
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
            }
            glfwWindowHint(GLFW_SAMPLES, 4);
            return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        }
    }

    Window::Window(int width, int height, const std::string& title)
        : width_(width), height_(height) {

        std::cout << "  [Window] Inicializando GLFW..." << std::endl;
        std::cout.flush();

        glfwSetErrorCallback(glfwErrorCallback);

        if (g_windowCount == 0) {
            if (!glfwInit()) {
                throw std::runtime_error("Falha ao inicializar GLFW");
            }
        }

        struct Attempt {
            int major;
            int minor;
            bool core;
            const char* label;
        };

        const Attempt attempts[] = {
            {3, 3, true,  "OpenGL 3.3 Core"},
            {3, 2, true,  "OpenGL 3.2 Core"},
            {3, 0, false, "OpenGL 3.0 Compatibility"},
            {2, 1, false, "OpenGL 2.1 Compatibility"},
        };

        handle_ = nullptr;
        for (const auto& a : attempts) {
            std::cout << "  [Window] Tentando " << a.label << "..." << std::endl;
            std::cout.flush();
            handle_ = tryCreateWindow(width, height, title, a.major, a.minor, a.core);
            if (handle_) {
                std::cout << "  [Window] Sucesso com " << a.label << "!" << std::endl;
                std::cout.flush();
                break;
            }
        }

        if (!handle_) {
            glfwTerminate();
            throw std::runtime_error("Falha: nenhuma versao de OpenGL suportada");
        }

        glfwMakeContextCurrent(handle_);
        glfwSetWindowUserPointer(handle_, this);
        glfwSetFramebufferSizeCallback(handle_, framebufferSizeCallback);
        glfwSwapInterval(1);

        std::cout << "  [Window] Inicializando GLEW..." << std::endl;
        std::cout.flush();

        glewExperimental = GL_TRUE;
        GLenum glewResult = glewInit();
        if (glewResult != GLEW_OK) {
            std::cerr << "  [Window] FALHA GLEW: "
                << reinterpret_cast<const char*>(glewGetErrorString(glewResult))
                << std::endl;
            std::cerr.flush();
            glfwDestroyWindow(handle_);
            glfwTerminate();
            throw std::runtime_error("Falha ao inicializar GLEW");
        }

        glGetError(); // limpa erro inicial inocuo

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);

        std::cout << "  [Window] OpenGL Vendor:   " << (vendor ? reinterpret_cast<const char*>(vendor) : "?") << std::endl;
        std::cout << "  [Window] OpenGL Renderer: " << (renderer ? reinterpret_cast<const char*>(renderer) : "?") << std::endl;
        std::cout << "  [Window] OpenGL Version:  " << (version ? reinterpret_cast<const char*>(version) : "?") << std::endl;
        std::cout << "  [Window] GLSL Version:    " << (glsl ? reinterpret_cast<const char*>(glsl) : "?") << std::endl;
        std::cout.flush();

        ++g_windowCount;
    }

    Window::~Window() {
        if (handle_) {
            glfwDestroyWindow(handle_);
        }
        --g_windowCount;
        if (g_windowCount == 0) {
            glfwTerminate();
        }
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(handle_);
    }

    void Window::requestClose() {
        glfwSetWindowShouldClose(handle_, GLFW_TRUE);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(handle_);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::framebufferSizeCallback(GLFWwindow* w, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        self->width_ = width;
        self->height_ = height;
        glViewport(0, 0, width, height);
    }

} // namespace platform