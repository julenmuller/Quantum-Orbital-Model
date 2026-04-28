#pragma once

#include "physics/ElementDatabase.hpp"
#include "physics/Orbital.hpp"
#include "physics/OrbitalSampler.hpp"
#include "platform/Window.hpp"
#include "rendering/AxisGizmo.hpp"
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
        void handleKeyboardShortcuts();
        void render();
        void render2D();
        void render3D();
        void renderUI();
        void renderSplashScreen();
        void regenerateOrbitalsIfNeeded();
        void resetCamera3D();
        void assignColorsToOrbitals();

        std::unique_ptr<platform::Window> window_;
        std::unique_ptr<ui::UIManager> uiManager_;

        physics::ElementDatabase elementDb_;
        physics::OrbitalSampler sampler_;

        rendering::Camera2D camera_;
        rendering::Camera3D camera3D_;

        rendering::ParticleRenderer particleRenderer_;
        rendering::OrbitRenderer orbitRenderer_;

        rendering::ParticleRenderer3D particleRenderer3D_;
        rendering::SphereWireframe sphereWireframe_;
        rendering::AxisGizmo axisGizmo_;

        ui::ControlPanel controlPanel_;
        ui::InfoPanel infoPanel_;
        ui::ControlState controlState_;

        std::shared_ptr<physics::Element> currentElement_;
        std::vector<std::shared_ptr<physics::Orbital>> currentOrbitals_;

        bool showSplash_ = true;
        bool prevTabKey_ = false;
        bool prevSpaceKey_ = false;
        bool prevRKey_ = false;
        float fps_ = 0.0f;
        float fpsAccumulator_ = 0.0f;
        int fpsFrames_ = 0;
        double lastTime_ = 0.0;
    };

}