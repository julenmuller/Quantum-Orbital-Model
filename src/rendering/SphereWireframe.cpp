#include "rendering/SphereWireframe.hpp"
#include <GL/glew.h>
#include <cmath>

namespace rendering {

    namespace {
        constexpr float TAU = 6.28318530718f;
        constexpr int SEGMENTS_PER_CIRCLE = 96;
    }

    SphereWireframe::SphereWireframe() = default;

    bool SphereWireframe::initialize(const std::string& shaderDir) {
        return shader_.loadFromFiles(
            shaderDir + "/sphere.vert",
            shaderDir + "/sphere.frag"
        );
    }

    void SphereWireframe::buildShells(const std::vector<float>& radii) {

        std::vector<float> data;

        auto pushPair = [&](float x0, float y0, float z0,
            float x1, float y1, float z1) {
                data.push_back(x0); data.push_back(y0); data.push_back(z0);
                data.push_back(x1); data.push_back(y1); data.push_back(z1);
            };

        for (float r : radii) {
            for (int i = 0; i < SEGMENTS_PER_CIRCLE; ++i) {
                const float a0 = TAU * i / SEGMENTS_PER_CIRCLE;
                const float a1 = TAU * (i + 1) / SEGMENTS_PER_CIRCLE;
                const float c0 = std::cos(a0), s0 = std::sin(a0);
                const float c1 = std::cos(a1), s1 = std::sin(a1);

                // Plano XY (z=0)
                pushPair(r * c0, r * s0, 0.0f, r * c1, r * s1, 0.0f);
                // Plano XZ (y=0)
                pushPair(r * c0, 0.0f, r * s0, r * c1, 0.0f, r * s1);
                // Plano YZ (x=0)
                pushPair(0.0f, r * c0, r * s0, 0.0f, r * c1, r * s1);
            }
        }

        buffer_.upload(data.data(), data.size(), {
            {0, 3, false}
            });
        vertexCount_ = static_cast<int>(data.size() / 3);
    }

    void SphereWireframe::render(const Camera3D& camera) {
        if (vertexCount_ == 0) return;

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader_.use();
        shader_.setMat4("uProjection", camera.projectionMatrix());
        shader_.setMat4("uView", camera.viewMatrix());

        buffer_.bind();
        glDrawArrays(GL_LINES, 0, vertexCount_);
        VertexBuffer::unbind();
    }

}