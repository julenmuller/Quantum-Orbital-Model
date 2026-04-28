#include "ui/ControlPanel.hpp"

#include <imgui.h>

namespace ui {

    void ControlPanel::render(ControlState& state, const physics::ElementDatabase& db) {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(340, 350), ImGuiCond_FirstUseEver);
        ImGui::Begin("Controles");

        // TabBar - escolhe entre 2D e 3D

        if (ImGui::BeginTabBar("ViewModeTabBar")) {
            if (ImGui::BeginTabItem("2D Bohr-Schrodinger")) {
                if (state.viewMode != app::ViewMode::View2D) {
                    state.viewMode = app::ViewMode::View2D;
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("3D Volumetrico")) {
                if (state.viewMode != app::ViewMode::View3D) {
                    state.viewMode = app::ViewMode::View3D;
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::Spacing();

        // Controles compartilhados entre 2D e 3D

        ImGui::TextColored(ImVec4(0.8f, 0.85f, 1.0f, 1.0f), "Elemento");
        ImGui::Separator();

        if (ImGui::BeginCombo("##ElementCombo", state.selectedSymbol.c_str())) {
            for (const auto& el : db.all()) {
                const bool selected = (state.selectedSymbol == el->symbol());
                std::string label = el->symbol() + " - " + el->name();
                if (ImGui::Selectable(label.c_str(), selected)) {
                    state.selectedSymbol = el->symbol();
                    state.needsRegeneration = true;
                }
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.8f, 0.85f, 1.0f, 1.0f), "Amostragem");
        ImGui::Separator();

        ImGui::SliderInt("Pontos/orbital", &state.particlesPerOrbital, 500, 50000);
        if (ImGui::Button("Regenerar nuvem", ImVec2(-1, 0))) {
            state.needsRegeneration = true;
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.8f, 0.85f, 1.0f, 1.0f), "Visualizacao");
        ImGui::Separator();

        ImGui::Checkbox("Mostrar camadas de Bohr", &state.showShells);
        ImGui::SliderFloat("Tamanho dos pontos", &state.pointSize, 1.0f, 10.0f);
        ImGui::Spacing();
        if (state.viewMode == app::ViewMode::View2D) {
            ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.4f, 1.0f), "Modo 2D");
            ImGui::Separator();
            ImGui::SliderFloat("Zoom", &state.zoom, 0.2f, 5.0f);
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.4f, 1.0f), "Modo 3D");
            ImGui::Separator();
            ImGui::SliderFloat("Distancia camera", &state.cameraDistance, 5.0f, 100.0f);
            ImGui::TextWrapped("Botao direito + arrastar para rotacionar.");
            ImGui::TextWrapped("Scroll do mouse para zoom.");
        }

        ImGui::End();
    }

}