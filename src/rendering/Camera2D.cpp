// ============================================================
// Camera2D.cpp
// ============================================================
#include "rendering/Camera2D.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace rendering {

    Camera2D::Camera2D() = default;

    void Camera2D::updateAspect(float aspect) {
        aspect_ = aspect;
    }

    void Camera2D::setWorldHeight(float height) {
        zoom_ = height / 2.0f;
    }

    glm::mat4 Camera2D::projectionMatrix() const {
        // Como o autor mencionou no cabeçalho: "o centro vai diretamente na projeção"
        float halfWidth = zoom_ * aspect_;
        float halfHeight = zoom_;

        return glm::ortho(
            center_.x - halfWidth,   // Esquerda
            center_.x + halfWidth,   // Direita
            center_.y - halfHeight,  // Baixo
            center_.y + halfHeight,  // Cima
            -1.0f,                   // Z-Near
            1.0f                     // Z-Far
        );
    }

    glm::mat4 Camera2D::viewMatrix() const {
        // Como a projeção já lida com o deslocamento da câmera (centro),
        // a view matrix pode ser apenas uma matriz identidade.
        return glm::mat4(1.0f);
    }

    glm::mat4 Camera2D::viewProjectionMatrix() const {
        return projectionMatrix() * viewMatrix();
    }

} // namespace rendering