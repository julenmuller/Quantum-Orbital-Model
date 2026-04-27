// ============================================================
// Application.hpp
// ------------------------------------------------------------
// Orquestra o ciclo de vida da aplicação:
//   1. Inicializa janela, OpenGL, ImGui
//   2. Carrega banco de elementos e descrições
//   3. Loop principal: input -> UI -> física -> render
//   4. Shutdown limpo
// ============================================================

#pragma once

#include "physics/ElementDatabase.hpp"
#include "physics/Orbital.hpp"
#include "physics/OrbitalSampler.hpp"
#include "platform/Window.hpp"
#include "rendering/Camera2D.hpp"
#include "rendering/OrbitRenderer.hpp"
#include "rendering/ParticleRenderer.hpp"
#include "ui/ControlPanel.hpp"
#include "ui/InfoPanel.hpp"
#include "ui/UIManager.hpp"

#include <memory>
#include <vector>

namespace app {

class Application {
public:
    Application();
    ~Application();

    // Inicializa todas as camadas. Retorna 0 em sucesso.
    int initialize();

    // Loop principal — bloqueia até o usuário fechar.
    void run();

private:
    // ----- Etapas internas do loop -----
    void update();
    void render();
    void regenerateOrbitalsIfNeeded();
    void assignColorsToOrbitals();

    // ----- Membros -----
    std::unique_ptr<platform::Window> window_;
    std::unique_ptr<ui::UIManager> uiManager_;

    physics::ElementDatabase elementDb_;
    physics::OrbitalSampler sampler_;

    rendering::Camera2D camera_;
    rendering::ParticleRenderer particleRenderer_;
    rendering::OrbitRenderer orbitRenderer_;

    ui::ControlPanel controlPanel_;
    ui::InfoPanel infoPanel_;
    ui::ControlState controlState_;

    std::shared_ptr<physics::Element> currentElement_;
    std::vector<std::shared_ptr<physics::Orbital>> currentOrbitals_;
};

} // namespace app
