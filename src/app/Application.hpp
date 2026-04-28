#pragma once

#include "physics/ElementDatabase.hpp"
#include "physics/Orbital.hpp"
#include "physics/OrbitalSampler.hpp"
#include "platform/Window.hpp"
#include "rendering/Camera2D.hpp"
#include "rendering/Camera3D.hpp"
#include "rendering/OrbitRenderer.hpp"
#include "rendering/ParticleRenderer.hpp"
#include "rendering/ParticleRenderer3D.hpp"
#include "rendering/SphereWireframe.hpp"
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
        void render2D();
        void render3D();   
        void regenerateOrbitalsIfNeeded();
        void assignColorsToOrbitals();

        std::unique_ptr<platform::Window> window_;
        std::unique_ptr<ui::UIManager> uiManager_;
        physics::ElementDatabase elementDb_;
        physics::OrbitalSampler sampler_;

        // Câmeras
        rendering::Camera2D camera_;
        rendering::Camera3D camera3D_;

        // Renderers 2D
        rendering::ParticleRenderer particleRenderer_;
        rendering::OrbitRenderer orbitRenderer_;

        // Renderers 3D
        rendering::ParticleRenderer3D particleRenderer3D_;
        rendering::SphereWireframe sphereWireframe_;
        ui::ControlPanel controlPanel_;
        ui::InfoPanel infoPanel_;
        ui::ControlState controlState_;
        std::shared_ptr<physics::Element> currentElement_;
        std::vector<std::shared_ptr<physics::Orbital>> currentOrbitals_;
    };

}