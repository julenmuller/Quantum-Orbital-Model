// ============================================================
// ParticleRenderer.hpp
// ============================================================

#pragma once

#include "physics/Orbital.hpp"
#include "rendering/Camera2D.hpp"
#include "rendering/Shader.hpp"
#include "rendering/VertexBuffer.hpp"

#include <memory>
#include <vector>

namespace rendering {

class ParticleRenderer {
public:
    ParticleRenderer();

    bool initialize(const std::string& shaderDir);

    void uploadOrbitals(const std::vector<std::shared_ptr<physics::Orbital>>& orbitals);

    void render(const Camera2D& camera, float pointSize = 3.0f);

private:
    Shader shader_;
    VertexBuffer buffer_;
    int totalPoints_ = 0;
};

} // namespace rendering
