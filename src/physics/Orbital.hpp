// Representa UM orbital específico (n, l, m). Guarda os
// pontos amostrados (cache) — em 2D E em 3D.

#pragma once

#include "physics/QuantumNumbers.hpp"
#include "physics/OrbitalSampler.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace physics {

    class Orbital {
    public:
        Orbital(const QuantumNumbers& qn, int Z = 1);

        // Gera os pontos 2D
        void generatePoints(OrbitalSampler& sampler, int numPoints,
            double samplingRadius = 30.0);

        // Gera os pontos 3D
        void generatePoints3D(OrbitalSampler& sampler, int numPoints,
            double samplingRadius = 30.0);

        const std::vector<OrbitalPoint>& points() const { return points_; }
        const std::vector<OrbitalPoint3D>& points3D() const { return points3D_; }

        const QuantumNumbers& quantumNumbers() const { return qn_; }
        int atomicNumber() const { return Z_; }

        void setColor(const glm::vec3& color) { color_ = color; }
        const glm::vec3& color() const { return color_; }

        void setOccupancy(int electrons) { occupancy_ = electrons; }
        int occupancy() const { return occupancy_; }

        std::string name() const { return qn_.spectroscopicName(); }

    private:
        QuantumNumbers qn_;
        int Z_;
        std::vector<OrbitalPoint> points_;       // 2D
        std::vector<OrbitalPoint3D> points3D_;   // 3D
        glm::vec3 color_{ 1.0f, 1.0f, 1.0f };
        int occupancy_ = 0;
    };

}