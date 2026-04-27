// ============================================================
// Application.cpp - VERSÃO COM DIAGNÓSTICO
// ============================================================

#include "app/Application.hpp"
#include "app/Config.hpp"
#include "physics/BohrModel.hpp"
#include "platform/Input.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

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

    } // namespace anônimo

    Application::Application() = default;

    Application::~Application() {
        if (uiManager_) uiManager_->shutdown();
    }

    int Application::initialize() {
        std::cout << "[1] Iniciando..." << std::endl;
        std::cout.flush();

        // ----- Janela + OpenGL -----
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
            std::cerr.flush();
            return 1;
        }

        std::cout << "[4] Inicializando ImGui..." << std::endl;
        std::cout.flush();
        uiManager_ = std::make_unique<ui::UIManager>();
        if (!uiManager_->initialize(*window_)) {
            std::cerr << "Falha ao inicializar Dear ImGui\n";
            std::cerr.flush();
            return 2;
        }
        std::cout << "[5] ImGui ok!" << std::endl;
        std::cout.flush();

        std::cout << "[6] Carregando elementos de " << config::elementsPath() << "..." << std::endl;
        std::cout.flush();
        if (!elementDb_.loadFromFile(config::elementsPath())) {
            std::cerr << "Falha ao carregar elements.json\n";
            std::cerr.flush();
            return 3;
        }
        std::cout << "[7] Elementos carregados!" << std::endl;
        std::cout.flush();

        std::cout << "[8] Carregando descricoes..." << std::endl;
        std::cout.flush();
        if (!infoPanel_.loadDescriptions(config::descriptionsPath())) {
            std::cerr << "Aviso: descricoes nao carregadas\n";
            std::cerr.flush();
        }
        std::cout << "[9] Descricoes ok" << std::endl;
        std::cout.flush();

        std::cout << "[10] Inicializando ParticleRenderer (shaders em " << config::shadersDir() << ")..." << std::endl;
        std::cout.flush();
        if (!particleRenderer_.initialize(config::shadersDir())) {
            std::cerr << "Falha no ParticleRenderer\n";
            std::cerr.flush();
            return 4;
        }
        std::cout << "[11] ParticleRenderer ok" << std::endl;
        std::cout.flush();

        std::cout << "[12] Inicializando OrbitRenderer..." << std::endl;
        std::cout.flush();
        if (!orbitRenderer_.initialize(config::shadersDir())) {
            std::cerr << "Falha no OrbitRenderer\n";
            std::cerr.flush();
            return 5;
        }
        std::cout << "[13] OrbitRenderer ok" << std::endl;
        std::cout.flush();

        std::cout << "[14] Configurando camera..." << std::endl;
        std::cout.flush();
        camera_.updateAspect(window_->aspectRatio());
        std::cout << "[15] Camera ok" << std::endl;
        std::cout.flush();

        std::cout << "[16] Gerando orbitais iniciais (Hidrogenio)..." << std::endl;
        std::cout.flush();
        controlState_.selectedSymbol = "H";
        controlState_.particlesPerOrbital = config::DEFAULT_PARTICLES_PER_ORBITAL;
        controlState_.needsRegeneration = true;
        regenerateOrbitalsIfNeeded();
        std::cout << "[17] Tudo pronto! Entrando no loop..." << std::endl;
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
        std::cout << "  [regen] " << currentOrbitals_.size() << " orbitais criados" << std::endl;
        std::cout.flush();

        assignColorsToOrbitals();

        int maxN = 1;
        for (const auto& shell : currentElement_->configuration()) {
            maxN = std::max(maxN, shell.n);
        }
        const double samplingRadius = std::max(15.0,
            2.0 * physics::BohrModel::shellRadius(maxN, currentElement_->atomicNumber()) + 5.0);

        std::cout << "  [regen] Amostrando pontos..." << std::endl;
        std::cout.flush();
        for (auto& orb : currentOrbitals_) {
            orb->generatePoints(sampler_,
                controlState_.particlesPerOrbital,
                samplingRadius);
        }
        std::cout << "  [regen] Upload pra GPU..." << std::endl;
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
        std::cout << "  [regen] OK" << std::endl;
        std::cout.flush();
    }

    void Application::update() {
        if (platform::Input::isKeyPressed(*window_, GLFW_KEY_ESCAPE)) {
            window_->requestClose();
        }

        regenerateOrbitalsIfNeeded();
        camera_.setZoom(controlState_.zoom);
        camera_.updateAspect(window_->aspectRatio());
    }

    void Application::render() {
        glClearColor(0.02f, 0.02f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (controlState_.showShells) {
            orbitRenderer_.render(camera_);
        }
        particleRenderer_.render(camera_, controlState_.pointSize);

        uiManager_->beginFrame();
        controlPanel_.render(controlState_, elementDb_);
        infoPanel_.render(currentElement_, currentOrbitals_);
        uiManager_->endFrame();
    }

    void Application::run() {
        while (!window_->shouldClose()) {
            window_->pollEvents();
            update();
            render();
            window_->swapBuffers();
        }
    }

} // namespace app