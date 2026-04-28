// ViewMode.hpp
// ------------------------------------------------------------
// Enum dos modos de visualização disponíveis na aplicação.
// O usuário escolhe via tabs na UI, e a Application usa esse
// valor pra decidir qual câmera/renderer usar no frame atual.

#pragma once

namespace app {

    enum class ViewMode {
        View2D,  // 2D (Bohr + corte de Schrödinger)
        View3D,  // 3D (nuvem volumétrica)
    };

    inline const char* viewModeName(ViewMode mode) {
        switch (mode) {
        case ViewMode::View2D: return "2D";
        case ViewMode::View3D: return "3D";
        }
        return "?";
    }

}