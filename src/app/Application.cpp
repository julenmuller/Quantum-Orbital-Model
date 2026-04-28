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

        glm::vec3 colorForOrbital(int l) {
            static const glm::vec3 palette[] = {
                {0.40f, 0.85f, 1.00f},
                {1.00f, 0.50f, 0.85f},
                {1.00f, 0.85f, 0.40f},
                {0.55f, 1.00f, 0.55f},
            };
            if (l < 0) l = 0;
            if (l > 3) l = 3;
            return palette[l];
        }

        constexpr float MOUSE_ROTATE_SENSITIVITY = 0.005f;
        constexpr float MOUSE_ZOOM_SENSITIVITY = 1.5f;     

    }

    Application::Application() = default;

    Application::~Application() {
        if (uiManager_) uiManager_->shutdown();
    }

    int Application::initialize() {
        std::cout << "[1] Iniciando..." << std::endl;
        std::cout.flush();

        try {
            std::cout << "[2] Criando janela..." << std::endl;
            std::cout.flush();
            window_ = std::make_unique<platform::Window>(
                config::WINDOW_WIDTH,
                config::WINDOW_HEIGHT,
                config::WINDOW_TITLE
            );
            std::cout << "[3] Janela criada!" << std::endl;
            std::cout.flush();
        }
        catch (const std::exception& ex) {
            std::cerr << "Erro fatal: " << ex.what() << "\n";
            return 1;
        }

        std::cout << "[4] Inicializando ImGui..." << std::endl;
        std::cout.flush();
        uiManager_ = std::make_unique<ui::UIManager>();
        if (!uiManager_->initialize(*window_)) {
            std::cerr << "Falha ao inicializar Dear ImGui\n";
            return 2;
        }

        std::cout << "[5] Carregando elementos..." << std::endl;
        std::cout.flush();
        if (!elementDb_.loadFromFile(config::elementsPath())) return 3;
        if (!infoPanel_.loadDescriptions(config::descriptionsPath())) {
            std::cerr << "Aviso: descricoes nao carregadas\n";
        }

        std::cout << "[6] Inicializando renderers 2D..." << std::endl;
        std::cout.flush();
        if (!particleRenderer_.initialize(config::shadersDir())) return 4;
        if (!orbitRenderer_.initialize(config::shadersDir())) return 5;

        std::cout << "[7] Configurando cameras..." << std::endl;
        std::cout.flush();
        camera_.updateAspect(window_->aspectRatio());
        camera3D_.updateAspect(window_->aspectRatio());

        std::cout << "[8] Gerando orbitais iniciais..." << std::endl;
        std::cout.flush();
        controlState_.selectedSymbol = "H";
        controlState_.particlesPerOrbital = config::DEFAULT_PARTICLES_PER_ORBITAL;
        controlState_.needsRegeneration = true;
        regenerateOrbitalsIfNeeded();
        std::cout << "[9] Tudo pronto!" << std::endl;
        std::cout.flush();

        return 0;
    }

    void Application::assignColorsToOrbitals() {
        for (auto& orb : currentOrbitals_) {
            orb->setColor(colorForOrbital(orb->quantumNumbers().l));
        }
    }

    void Application::regenerateOrbitalsIfNeeded() {
        if (!controlState_.needsRegeneration) return;
        controlState_.needsRegeneration = false;

        currentElement_ = elementDb_.get(controlState_.selectedSymbol);
        if (!currentElement_) return;

        std::cout << "  [regen] Gerando orbitais de " << currentElement_->name() << std::endl;
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
        std::cout << "  [regen] Pontos gerados: 2D=" << total2D
            << ", 3D=" << total3D << std::endl;
        std::cout.flush();

        particleRenderer_.uploadOrbitals(currentOrbitals_);

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

        if (std::abs(controlState_.cameraDistance - camera3D_.distance()) > 0.01f) {
            controlState_.cameraDistance = camera3D_.distance();
        }
    }

    void Application::render() {
        glClearColor(0.02f, 0.02f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (controlState_.viewMode == ViewMode::View2D) {
            if (controlState_.showShells) {
                orbitRenderer_.render(camera_);
            }
            particleRenderer_.render(camera_, controlState_.pointSize);
        }
        else {
        }

        uiManager_->beginFrame();
        controlPanel_.render(controlState_, elementDb_);
        infoPanel_.render(currentElement_, currentOrbitals_);
        if (controlState_.viewMode == ViewMode::View3D) {
            ImGui::SetNextWindowPos(
                ImVec2(window_->width() / 2.0f - 220, window_->height() / 2.0f - 90),
                ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(440, 180), ImGuiCond_Always);
            ImGui::Begin("3D - Fase 3 (Camera)", nullptr,
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse);

            const float yawDeg = camera3D_.yaw() * 57.29578f;
            const float pitchDeg = camera3D_.pitch() * 57.29578f;
            const glm::vec3 pos = camera3D_.position();

            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f),
                "Camera 3D ATIVA");
            ImGui::Text("Yaw:   %.1f deg", yawDeg);
            ImGui::Text("Pitch: %.1f deg", pitchDeg);
            ImGui::Text("Distance: %.2f bohr", camera3D_.distance());
            ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
            ImGui::Separator();
            ImGui::TextWrapped("Botao DIREITO + arrastar = rotacionar");
            ImGui::TextWrapped("Scroll do mouse = zoom");
            ImGui::TextWrapped("Renderer 3D vem na proxima fase.");

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