#pragma once

#include "rendering/Camera3D.hpp"
#include "rendering/Shader.hpp"
#include "rendering/VertexBuffer.hpp"

#include <vector>

namespace rendering {

    class SphereWireframe {
    public:
        SphereWireframe();

        bool initialize(const std::string& shaderDir);
        void buildShells(const std::vector<float>& radii);
        void render(const Camera3D& camera);

    private:
        Shader shader_;
        VertexBuffer buffer_;
        int vertexCount_ = 0;
    };

}