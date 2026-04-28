#include "rendering/Camera3D.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <cmath>

namespace rendering {

    namespace {
        constexpr float PITCH_LIMIT = 1.553f;
        constexpr float MIN_DISTANCE = 1.0f;
        constexpr float MAX_DISTANCE = 200.0f;
    }

    Camera3D::Camera3D() {
        recompute();
    }

    void Camera3D::updateAspect(float aspectRatio) {
        aspectRatio_ = aspectRatio;
        recompute();
    }

    void Camera3D::setYaw(float yawRadians) {
        yaw_ = yawRadians;
        recompute();
    }

    void Camera3D::setPitch(float pitchRadians) {
        pitch_ = std::clamp(pitchRadians, -PITCH_LIMIT, PITCH_LIMIT);
        recompute();
    }

    void Camera3D::setDistance(float distance) {
        distance_ = std::clamp(distance, MIN_DISTANCE, MAX_DISTANCE);
        recompute();
    }

    void Camera3D::rotate(float deltaYaw, float deltaPitch) {
        yaw_ += deltaYaw;
        if (yaw_ > glm::two_pi<float>()) yaw_ -= glm::two_pi<float>();
        if (yaw_ < 0.0f) yaw_ += glm::two_pi<float>();

        pitch_ = std::clamp(pitch_ + deltaPitch, -PITCH_LIMIT, PITCH_LIMIT);
        recompute();
    }

    void Camera3D::zoom(float distanceDelta) {
        distance_ = std::clamp(distance_ + distanceDelta, MIN_DISTANCE, MAX_DISTANCE);
        recompute();
    }

    void Camera3D::setFOV(float fovDegrees) {
        fov_ = fovDegrees;
        recompute();
    }

    void Camera3D::setNearFar(float nearPlane, float farPlane) {
        near_ = nearPlane;
        far_ = farPlane;
        recompute();
    }

    glm::vec3 Camera3D::position() const {
        const float cp = std::cos(pitch_);
        const float sp = std::sin(pitch_);
        const float cy = std::cos(yaw_);
        const float sy = std::sin(yaw_);
        return glm::vec3(
            distance_ * cp * sy,
            distance_ * sp,
            distance_ * cp * cy
        );
    }

    void Camera3D::recompute() {
        const glm::vec3 pos = position();
        const glm::vec3 target(0.0f);
        const glm::vec3 up(0.0f, 1.0f, 0.0f);

        view_ = glm::lookAt(pos, target, up);
        projection_ = glm::perspective(glm::radians(fov_), aspectRatio_, near_, far_);
    }

}