#pragma once

#include "app/ViewMode.hpp"
#include "physics/ElementDatabase.hpp"

#include <memory>
#include <string>

namespace ui {

    struct ControlState {
        // Modo de visualização
        app::ViewMode viewMode = app::ViewMode::View2D;

        // Estado compartilhado entre 2D e 3D 
        std::string selectedSymbol = "H";
        int particlesPerOrbital = 5000;
        bool showShells = true;
        bool needsRegeneration = false;
        float pointSize = 3.0f;

        // Específico do modo 2D
        float zoom = 1.0f;

        // Específico do modo 3D
        float cameraDistance = 25.0f;
        float cameraYaw = 0.0f;    // rotação horizontal (radianos)
        float cameraPitch = 0.3f;  // rotação vertical (radianos)
    };

    class ControlPanel {
    public:
        void render(ControlState& state, const physics::ElementDatabase& db);
    };

}
