#pragma once

#include "physics/ElementDatabase.hpp"
#include "physics/Orbital.hpp"
#include "physics/OrbitalSampler.hpp"
#include "platform/Window.hpp"
#include "rendering/Camera2D.hpp"
#include "rendering/Camera3D.hpp"
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

        int initialize();
        void run();

    private:
        void update();
        void update3DCamera();
        void render();
        void regenerateOrbitalsIfNeeded();
        void assignColorsToOrbitals();

        std::unique_ptr<platform::Window> window_;
        std::unique_ptr<ui::UIManager> uiManager_;

        physics::ElementDatabase elementDb_;
        physics::OrbitalSampler sampler_;

        rendering::Camera2D camera_;
        rendering::Camera3D camera3D_;
        rendering::ParticleRenderer particleRenderer_;
        rendering::OrbitRenderer orbitRenderer_;

        ui::ControlPanel controlPanel_;
        ui::InfoPanel infoPanel_;
        ui::ControlState controlState_;

        std::shared_ptr<physics::Element> currentElement_;
        std::vector<std::shared_ptr<physics::Orbital>> currentOrbitals_;
    };

}