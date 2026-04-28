#include "rendering/AxisGizmo.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace rendering {

    AxisGizmo::AxisGizmo() = default;

    bool AxisGizmo::initialize(const std::string& shaderDir) {
        bool ok = shader_.loadFromFiles(
            shaderDir + "/axis.vert",
            shaderDir + "/axis.frag"
        );
        if (!ok) return false;
        buildGeometry();
        return true;
    }

    void AxisGizmo::buildGeometry() {
        const float L = 1.0f;
        const float verts[] = {

            0, 0, 0,   1, 0.2f, 0.2f,
            L, 0, 0,   1, 0.2f, 0.2f,

            0, 0, 0,   0.2f, 1, 0.2f,
            0, L, 0,   0.2f, 1, 0.2f,

            0, 0, 0,   0.4f, 0.5f, 1,
            0, 0, L,   0.4f, 0.5f, 1,
        };

        buffer_.upload(verts, sizeof(verts) / sizeof(float), {
            {0, 3, false},
            {1, 3, false}   
            });
        vertexCount_ = 6;
    }

    void AxisGizmo::render(const Camera3D& camera, int fbWidth, int fbHeight) {
        if (vertexCount_ == 0) return;

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2.5f);

        const int gizmoSize = 90;
        const int gx = fbWidth - gizmoSize - 12;
        const int gy = 12;
        glViewport(gx, gy, gizmoSize, gizmoSize);
        glm::mat4 view = camera.viewMatrix();
        view[3] = glm::vec4(0, 0, -3.0f, 1.0f);

        glm::mat4 proj = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, 0.1f, 10.0f);

        shader_.use();
        shader_.setMat4("uProjection", proj);
        shader_.setMat4("uView", view);

        buffer_.bind();
        glDrawArrays(GL_LINES, 0, vertexCount_);
        VertexBuffer::unbind();

        glViewport(0, 0, fbWidth, fbHeight);
        glLineWidth(1.0f);
    }

}