#pragma once

#include <glm/glm.hpp>

namespace rendering {

    class Camera3D {
    public:
        Camera3D();
        void updateAspect(float aspectRatio);

        // Setters
        void setYaw(float yawRadians);
        void setPitch(float pitchRadians);
        void setDistance(float distance);

        // Deltas
        void rotate(float deltaYaw, float deltaPitch);
        void zoom(float distanceDelta);

        // Getters
        float yaw() const { return yaw_; }
        float pitch() const { return pitch_; }
        float distance() const { return distance_; }
        glm::vec3 position() const;

        const glm::mat4& projectionMatrix() const { return projection_; }
        const glm::mat4& viewMatrix() const { return view_; }

        // Configuração
        void setFOV(float fovDegrees);
        void setNearFar(float nearPlane, float farPlane);

    private:
        void recompute();

        float aspectRatio_ = 1.0f;
        float fov_ = 45.0f;  
        float near_ = 0.1f;
        float far_ = 500.0f;

        float yaw_ = 0.0f;        
        float pitch_ = 0.3f;     
        float distance_ = 25.0f;  

        glm::mat4 projection_{ 1.0f };
        glm::mat4 view_{ 1.0f };
    };

}