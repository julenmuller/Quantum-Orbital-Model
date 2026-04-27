// ============================================================
// Config.hpp
// ============================================================

#pragma once

#include <string>

namespace app {

namespace config {
    constexpr int WINDOW_WIDTH  = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr const char* WINDOW_TITLE = "Quantum Orbital Model - 2D";

    constexpr int DEFAULT_PARTICLES_PER_ORBITAL = 5000;
    constexpr double DEFAULT_SAMPLING_RADIUS = 25.0;

    inline std::string elementsPath() { return "assets/data/elements.json"; }
    inline std::string descriptionsPath() { return "assets/data/orbital_descriptions.json"; }
    inline std::string shadersDir() { return "assets/shaders"; }
}

} // namespace app
