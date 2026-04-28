#pragma once

#include "rendering/Camera3D.hpp"
#include "rendering/Shader.hpp"
#include "rendering/VertexBuffer.hpp"

namespace rendering {

    class AxisGizmo {
    public:
        AxisGizmo();
        bool initialize(const std::string& shaderDir);
        void render(const Camera3D& camera, int fbWidth, int fbHeight);

    private:
        void buildGeometry();

        Shader shader_;
        VertexBuffer buffer_;
        int vertexCount_ = 0;
    };

}