// ============================================================
// OrbitalSampler.cpp
// ============================================================

#include "physics/OrbitalSampler.hpp"
#include "physics/Wavefunction.hpp"

#include <algorithm>
#include <cmath>

namespace physics {

OrbitalSampler::OrbitalSampler()
    : rng_(std::random_device{}())
{}

double OrbitalSampler::estimateMaxProbability(const QuantumNumbers& qn,
                                               int Z,
                                               double samplingRadius) {
    // Faz um grid coarse sobre a região, calcula |ψ|² em cada ponto,
    // retorna o máximo encontrado (com uma margem de segurança).
    double maxProb = 0.0;
    constexpr int GRID_STEPS = 80;
    const double step = 2.0 * samplingRadius / GRID_STEPS;

    for (int i = 0; i <= GRID_STEPS; ++i) {
        for (int j = 0; j <= GRID_STEPS; ++j) {
            const double x = -samplingRadius + i * step;
            const double z = -samplingRadius + j * step;
            const double p = Wavefunction::probabilityDensity2D(qn, x, z, Z);
            if (p > maxProb) maxProb = p;
        }
    }
    return maxProb * 1.05;  // 5% de margem
}

std::vector<OrbitalPoint> OrbitalSampler::sample(const QuantumNumbers& qn,
                                                  int numPoints,
                                                  int Z,
                                                  double samplingRadius) {
    std::vector<OrbitalPoint> points;
    points.reserve(numPoints);

    if (!qn.isValid() || numPoints <= 0) return points;

    const double maxProb = estimateMaxProbability(qn, Z, samplingRadius);
    if (maxProb <= 0.0) return points;

    std::uniform_real_distribution<double> uni(-samplingRadius, samplingRadius);
    std::uniform_real_distribution<double> uprob(0.0, maxProb);

    // Limite de tentativas para evitar loops infinitos em orbitais esparsos
    const int maxAttempts = numPoints * 1000;
    int attempts = 0;

    while (static_cast<int>(points.size()) < numPoints && attempts < maxAttempts) {
        ++attempts;
        const double x = uni(rng_);
        const double z = uni(rng_);
        const double p = Wavefunction::probabilityDensity2D(qn, x, z, Z);

        if (uprob(rng_) < p) {
            OrbitalPoint pt;
            pt.x = static_cast<float>(x);
            pt.z = static_cast<float>(z);
            // Intensidade normalizada pra deixar visualmente balanceado
            pt.intensity = static_cast<float>(std::min(1.0, p / maxProb));
            points.push_back(pt);
        }
    }

    return points;
}

} // namespace physics
