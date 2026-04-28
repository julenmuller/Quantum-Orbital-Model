#pragma once

#include "physics/Orbital.hpp"
#include "rendering/Camera3D.hpp"
#include "rendering/Shader.hpp"
#include "rendering/VertexBuffer.hpp"

#include <memory>
#include <vector>

namespace rendering {

    class ParticleRenderer3D {
    public:
        ParticleRenderer3D();
        bool initialize(const std::string& shaderDir);
        void uploadOrbitals(const std::vector<std::shared_ptr<physics::Orbital>>& orbitals);
        void render(const Camera3D& camera, float pointSize = 4.0f);

    private:
        Shader shader_;
        VertexBuffer buffer_;
        int totalPoints_ = 0;
    };

}