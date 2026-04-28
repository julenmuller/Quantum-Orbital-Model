#include "app/Application.hpp"
#include "app/Config.hpp"
#include "app/ViewMode.hpp"
#include "physics/BohrModel.hpp"
#include "platform/Input.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <imgui.h>
#include <iostream>
#include <set>

namespace app {

    namespace {
        glm::vec3 colorForOrbital(int n, int l) {

            glm::vec3 base;
            switch (l) {
            case 0: base = glm::vec3(0.40f, 0.85f, 1.00f); break;
            case 1: base = glm::vec3(1.00f, 0.50f, 0.85f); break;  
            case 2: base = glm::vec3(1.00f, 0.85f, 0.40f); break;  
            case 3: base = glm::vec3(0.55f, 1.00f, 0.55f); break;  
            default: base = glm::vec3(0.8f); break;
            }

            if (n == 1) {
                return base;
            }
            else if (n == 2) {
                return base * 0.75f + glm::vec3(0.05f);
            }
            else if (n == 3) {
                return base * 0.55f + glm::vec3(0.10f);
            }
            return base * 0.4f;
        }

        constexpr float MOUSE_ROTATE_SENSITIVITY = 0.005f;
        constexpr float MOUSE_ZOOM_SENSITIVITY = 1.5f;

    }

    Application::Application() = default;

    Application::~Application() {
        if (uiManager_) uiManager_->shutdown();
    }

    int Application::initialize() {
        std::cout << "[*] Iniciando..." << std::endl;
        std::cout.flush();

        try {
            window_ = std::make_unique<platform::Window>(
                config::WINDOW_WIDTH,
                config::WINDOW_HEIGHT,
                config::WINDOW_TITLE
            );
        }
        catch (const std::exception& ex) {
            std::cerr << "Erro fatal: " << ex.what() << "\n";
            return 1;
        }

        uiManager_ = std::make_unique<ui::UIManager>();
        if (!uiManager_->initialize(*window_)) return 2;

        if (!elementDb_.loadFromFile(config::elementsPath())) return 3;
        infoPanel_.loadDescriptions(config::descriptionsPath());

        if (!particleRenderer_.initialize(config::shadersDir())) return 4;
        if (!orbitRenderer_.initialize(config::shadersDir())) return 5;

        if (!particleRenderer3D_.initialize(config::shadersDir())) return 6;
        if (!sphereWireframe_.initialize(config::shadersDir())) return 7;

        camera_.updateAspect(window_->aspectRatio());
        camera3D_.updateAspect(window_->aspectRatio());

        controlState_.selectedSymbol = "H";
        controlState_.particlesPerOrbital = config::DEFAULT_PARTICLES_PER_ORBITAL;
        controlState_.pointSize = 4.0f;
        controlState_.needsRegeneration = true;
        regenerateOrbitalsIfNeeded();

        return 0;
    }

    void Application::assignColorsToOrbitals() {
        for (auto& orb : currentOrbitals_) {
            const auto& qn = orb->quantumNumbers();
            orb->setColor(colorForOrbital(qn.n, qn.l));
        }
    }

    void Application::regenerateOrbitalsIfNeeded() {
        if (!controlState_.needsRegeneration) return;
        controlState_.needsRegeneration = false;

        currentElement_ = elementDb_.get(controlState_.selectedSymbol);
        if (!currentElement_) return;

        std::cout << "  [regen] " << currentElement_->name() << "..." << std::endl;
        std::cout.flush();

        currentOrbitals_ = currentElement_->createOrbitals();
        assignColorsToOrbitals();

        int maxN = 1;
        for (const auto& shell : currentElement_->configuration()) {
            maxN = std::max(maxN, shell.n);
        }
        const double samplingRadius = std::max(15.0,
            2.0 * physics::BohrModel::shellRadius(maxN, currentElement_->atomicNumber()) + 5.0);

        for (auto& orb : currentOrbitals_) {
            orb->generatePoints(sampler_,
                controlState_.particlesPerOrbital,
                samplingRadius);
            orb->generatePoints3D(sampler_,
                controlState_.particlesPerOrbital,
                samplingRadius);
        }

        int total2D = 0, total3D = 0;
        for (const auto& orb : currentOrbitals_) {
            total2D += static_cast<int>(orb->points().size());
            total3D += static_cast<int>(orb->points3D().size());
        }
        std::cout << "  [regen] Pontos: 2D=" << total2D
            << ", 3D=" << total3D << std::endl;
        std::cout.flush();

        particleRenderer_.uploadOrbitals(currentOrbitals_);
        particleRenderer3D_.uploadOrbitals(currentOrbitals_);

        std::set<int> usedN;
        for (const auto& shell : currentElement_->configuration()) {
            usedN.insert(shell.n);
        }
        std::vector<float> radii;
        for (int n : usedN) {
            radii.push_back(static_cast<float>(
                physics::BohrModel::shellRadius(n, currentElement_->atomicNumber())));
        }
        orbitRenderer_.buildShells(radii);
        sphereWireframe_.buildShells(radii);

        camera_.setWorldHeight(static_cast<float>(samplingRadius * 2.2));
        camera3D_.setDistance(static_cast<float>(samplingRadius * 1.5));
    }

    void Application::update3DCamera() {
        auto& mouse = platform::Input::mouse();
        if (uiManager_->wantsCaptureMouse()) return;

        if (mouse.rightPressed && (mouse.deltaX != 0.0 || mouse.deltaY != 0.0)) {
            const float dyaw = -static_cast<float>(mouse.deltaX) * MOUSE_ROTATE_SENSITIVITY;
            const float dpitch = -static_cast<float>(mouse.deltaY) * MOUSE_ROTATE_SENSITIVITY;
            camera3D_.rotate(dyaw, dpitch);
        }

        if (mouse.scrollDelta != 0.0) {
            camera3D_.zoom(-static_cast<float>(mouse.scrollDelta) * MOUSE_ZOOM_SENSITIVITY);
        }
    }

    void Application::update() {
        if (platform::Input::isKeyPressed(*window_, GLFW_KEY_ESCAPE)) {
            window_->requestClose();
        }

        regenerateOrbitalsIfNeeded();

        camera_.setZoom(controlState_.zoom);
        camera_.updateAspect(window_->aspectRatio());

        camera3D_.updateAspect(window_->aspectRatio());
        if (controlState_.viewMode == ViewMode::View3D) {
            update3DCamera();
        }

        controlState_.cameraDistance = camera3D_.distance();
    }

    void Application::render2D() {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (controlState_.showShells) {
            orbitRenderer_.render(camera_);
        }
        particleRenderer_.render(camera_, controlState_.pointSize);
    }

    void Application::render3D() {
        if (controlState_.showShells) {
            sphereWireframe_.render(camera3D_);
        }
        particleRenderer3D_.render(camera3D_, controlState_.pointSize);
    }

    void Application::render() {
        glClearColor(0.02f, 0.02f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (controlState_.viewMode == ViewMode::View2D) {
            render2D();
        }
        else {
            render3D();
        }

        glDisable(GL_DEPTH_TEST);

        uiManager_->beginFrame();
        controlPanel_.render(controlState_, elementDb_);
        infoPanel_.render(currentElement_, currentOrbitals_);

        if (controlState_.viewMode == ViewMode::View3D) {
            ImGui::SetNextWindowPos(
                ImVec2(window_->width() - 230.0f, 10.0f),
                ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(220, 130), ImGuiCond_FirstUseEver);
            ImGui::Begin("Camera 3D");

            const float yawDeg = camera3D_.yaw() * 57.29578f;
            const float pitchDeg = camera3D_.pitch() * 57.29578f;

            ImGui::Text("Yaw:   %.1f deg", yawDeg);
            ImGui::Text("Pitch: %.1f deg", pitchDeg);
            ImGui::Text("Dist:  %.2f bohr", camera3D_.distance());
            ImGui::Separator();
            ImGui::TextWrapped("RMB+arrastar: girar");
            ImGui::TextWrapped("Scroll: zoom");

            ImGui::End();
        }

        uiManager_->endFrame();
    }

    void Application::run() {
        while (!window_->shouldClose()) {
            window_->pollEvents();
            update();
            render();
            window_->swapBuffers();
            platform::Input::resetFrameDeltas();
        }
    }

}