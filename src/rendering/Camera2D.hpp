// ============================================================
// Camera2D.hpp (Versão Corrigida)
// ============================================================
#pragma once

#include <glm/glm.hpp>

namespace rendering {

    class Camera2D {
    public:
        Camera2D();

        void setCenter(const glm::vec2& center) { center_ = center; }
        void setZoom(float zoom) { zoom_ = zoom; }
        void setAspectRatio(float aspect) { aspect_ = aspect; }

        // --- FUNÇÕES ADICIONADAS PARA COMPATIBILIDADE ---
        // O Application.cpp chama estas funções:
        void updateAspect(float aspect);
        void setWorldHeight(float height);
        // ------------------------------------------------

        const glm::vec2& center() const { return center_; }
        float zoom() const { return zoom_; }
        float aspectRatio() const { return aspect_; }

        // Matrizes pro shader
        glm::mat4 projectionMatrix() const;
        glm::mat4 viewMatrix() const;
        glm::mat4 viewProjectionMatrix() const;

    private:
        glm::vec2 center_{ 0.0f, 0.0f };
        float zoom_ = 30.0f;
        float aspect_ = 16.0f / 9.0f;
    };

} // namespace rendering