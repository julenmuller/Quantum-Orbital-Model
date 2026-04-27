// ============================================================
// OrbitRenderer.cpp
// ============================================================
#include "rendering/OrbitRenderer.hpp"
#include <GL/glew.h>
#include <cmath>
#include <vector>

namespace rendering {
    namespace {
        constexpr float TAU = 6.28318530718f;
        constexpr int SEGMENTS_PER_RING = 128;
    }

    OrbitRenderer::OrbitRenderer() = default;

    // Destrutor adicionado para resolver o erro LNK2019
    OrbitRenderer::~OrbitRenderer() {
    }

    bool OrbitRenderer::initialize(const std::string& shaderDir) {
        return shader_.loadFromFiles(
            shaderDir + "/orbit.vert",
            shaderDir + "/orbit.frag"
        );
    }

    void OrbitRenderer::buildShells(const std::vector<float>& radii) {
        // Geramos pares (linha) por segmento para usar GL_LINES.
        std::vector<float> data;
        for (float r : radii) {
            for (int i = 0; i < SEGMENTS_PER_RING; ++i) {
                float a0 = TAU * i / SEGMENTS_PER_RING;
                float a1 = TAU * (i + 1) / SEGMENTS_PER_RING;
                data.push_back(r * std::cos(a0));
                data.push_back(r * std::sin(a0));
                data.push_back(r * std::cos(a1));
                data.push_back(r * std::sin(a1));
            }
        }

        buffer_.upload(data.data(), data.size(), {
            {0, 2, false}
            });
        vertexCount_ = static_cast<int>(data.size() / 2);
    }

    void OrbitRenderer::render(const Camera2D& camera) {
        if (vertexCount_ == 0) return;

        shader_.use();
        shader_.setMat4("uProjection", camera.projectionMatrix());
        shader_.setMat4("uView", camera.viewMatrix());

        buffer_.bind();
        glDrawArrays(GL_LINES, 0, vertexCount_);
        VertexBuffer::unbind();
    }

} // namespace rendering