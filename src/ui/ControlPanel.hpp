// ============================================================
// ControlPanel.hpp
// ------------------------------------------------------------
// Painel ImGui para selecionar elemento, ajustar contagem
// de partículas e ligar/desligar visualizações auxiliares.
// ============================================================

#pragma once

#include "physics/ElementDatabase.hpp"

#include <memory>
#include <string>

namespace ui {

struct ControlState {
    std::string selectedSymbol = "H";  // elemento atual
    int particlesPerOrbital = 5000;    // pontos por orbital
    bool showShells = true;            // mostrar anéis de Bohr
    bool needsRegeneration = false;    // flag pra app saber que precisa re-amostrar
    float pointSize = 3.0f;            // tamanho dos pontos na tela
    float zoom = 1.0f;                 // zoom da câmera
};

class ControlPanel {
public:
    void render(ControlState& state, const physics::ElementDatabase& db);
};

} // namespace ui
