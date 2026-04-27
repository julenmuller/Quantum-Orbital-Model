// ============================================================
// ControlPanel.cpp
// ============================================================

#include "ui/ControlPanel.hpp"

#include <imgui.h>

namespace ui {

void ControlPanel::render(ControlState& state, const physics::ElementDatabase& db) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(320, 280), ImGuiCond_FirstUseEver);
    ImGui::Begin(u8"Controles");

    // ----- Seleção de elemento -----
    ImGui::TextColored(ImVec4(0.8f, 0.85f, 1.0f, 1.0f), u8"Elemento");
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

    // ----- Pontos por orbital -----
    ImGui::TextColored(ImVec4(0.8f, 0.85f, 1.0f, 1.0f), u8"Amostragem");
    ImGui::Separator();

    if (ImGui::SliderInt("Pontos/orbital", &state.particlesPerOrbital, 500, 50000)) {
        // não aplica imediato; usa botão pra regerar
    }
    if (ImGui::Button(u8"Regenerar nuvem", ImVec2(-1, 0))) {
        state.needsRegeneration = true;
    }

    ImGui::Spacing();

    // ----- Visualização -----
    ImGui::TextColored(ImVec4(0.8f, 0.85f, 1.0f, 1.0f), u8"Visualização");
    ImGui::Separator();

    ImGui::Checkbox(u8"Mostrar camadas de Bohr", &state.showShells);
    ImGui::SliderFloat("Tamanho dos pontos", &state.pointSize, 1.0f, 10.0f);
    ImGui::SliderFloat("Zoom", &state.zoom, 0.2f, 5.0f);

    ImGui::End();
}

} // namespace ui
