// ============================================================
// UIManager.cpp
// ============================================================

#include "ui/UIManager.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace ui {

UIManager::UIManager() = default;

UIManager::~UIManager() {
    if (initialized_) shutdown();
}

bool UIManager::initialize(platform::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;

    if (!ImGui_ImplGlfw_InitForOpenGL(window.nativeHandle(), true)) return false;
    if (!ImGui_ImplOpenGL3_Init("#version 330 core")) return false;

    initialized_ = true;
    return true;
}

void UIManager::shutdown() {
    if (!initialized_) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    initialized_ = false;
}

void UIManager::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::wantsCaptureMouse() const {
    return ImGui::GetIO().WantCaptureMouse;
}

bool UIManager::wantsCaptureKeyboard() const {
    return ImGui::GetIO().WantCaptureKeyboard;
}

} // namespace ui
