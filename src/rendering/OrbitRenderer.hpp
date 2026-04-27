#pragma once

#include "rendering/Shader.hpp"
#include "rendering/Camera2D.hpp"
#include "rendering/VertexBuffer.hpp"
#include <string>
#include <vector> // Necessário para o std::vector

namespace rendering {

    class OrbitRenderer {
    public:
        OrbitRenderer();
        ~OrbitRenderer();

        bool initialize(const std::string& shaderPath);

        // cabeçalho sabe que a função recebe os raios (radii)
        void buildShells(const std::vector<float>& radii);

        void render(const Camera2D& camera);

    private:
        Shader shader_;
        VertexBuffer buffer_;
        unsigned int vertexCount_;
    };

} // namespace rendering