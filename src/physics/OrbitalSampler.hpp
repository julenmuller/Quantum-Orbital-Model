// Gera pontos amostrados da densidade de probabilidade |ψ|²
// usando Rejection Sampling.
// Suporta dois modos:
//   - sample(): pontos no plano 2D (xz, com y=0)
//   - sample3D(): pontos no volume 3D

#pragma once

#include "physics/QuantumNumbers.hpp"

#include <vector>
#include <random>

namespace physics {

    // Ponto 2D
    struct OrbitalPoint {
        float x;
        float z;
        float intensity;
    };

    // Ponto 3D
    struct OrbitalPoint3D {
        float x;
        float y;
        float z;
        float intensity;
    };

    class OrbitalSampler {
    public:
        OrbitalSampler();

        //2D
        std::vector<OrbitalPoint> sample(const QuantumNumbers& qn,
            int numPoints,
            int Z = 1,
            double samplingRadius = 30.0);

        //3D
        std::vector<OrbitalPoint3D> sample3D(const QuantumNumbers& qn,
            int numPoints,
            int Z = 1,
            double samplingRadius = 30.0);

    private:
        // Estima máximo de |ψ|² em 2D
        double estimateMaxProbability(const QuantumNumbers& qn,
            int Z,
            double samplingRadius);

        // Estima máximo de |ψ|² em 3D
        double estimateMaxProbability3D(const QuantumNumbers& qn,
            int Z,
            double samplingRadius);

        std::mt19937 rng_;
    };

}