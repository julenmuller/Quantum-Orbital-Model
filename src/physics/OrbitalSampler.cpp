#include "physics/OrbitalSampler.hpp"
#include "physics/Wavefunction.hpp"

#include <algorithm>
#include <cmath>

namespace physics {

    OrbitalSampler::OrbitalSampler()
        : rng_(std::random_device{}())
    {
    }

    // 2D

    double OrbitalSampler::estimateMaxProbability(const QuantumNumbers& qn,
        int Z,
        double samplingRadius) {
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
        return maxProb * 1.05;
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
                pt.intensity = static_cast<float>(std::min(1.0, p / maxProb));
                points.push_back(pt);
            }
        }

        return points;
    }


    // 3D

    namespace {
        // Calcula |ψ|² em um ponto 3D (x, y, z)
        double probabilityDensity3D(const QuantumNumbers& qn,
            double x, double y, double z, int Z) {
            const double r = std::sqrt(x * x + y * y + z * z);
            if (r < 1e-12) return 0.0;
            const double theta = std::acos(std::clamp(z / r, -1.0, 1.0));
            const double phi = std::atan2(y, x);
            const double psi = Wavefunction::evaluate(qn, r, theta, phi, Z);
            return psi * psi;
        }
    }

    double OrbitalSampler::estimateMaxProbability3D(const QuantumNumbers& qn,
        int Z,
        double samplingRadius) {
        // Grid 3D coarse (40^3 = 64000 pontos pra encontrar o máximo)
        double maxProb = 0.0;
        constexpr int GRID_STEPS = 40;
        const double step = 2.0 * samplingRadius / GRID_STEPS;

        for (int i = 0; i <= GRID_STEPS; ++i) {
            for (int j = 0; j <= GRID_STEPS; ++j) {
                for (int k = 0; k <= GRID_STEPS; ++k) {
                    const double x = -samplingRadius + i * step;
                    const double y = -samplingRadius + j * step;
                    const double z = -samplingRadius + k * step;
                    const double p = probabilityDensity3D(qn, x, y, z, Z);
                    if (p > maxProb) maxProb = p;
                }
            }
        }
        return maxProb * 1.05;
    }

    std::vector<OrbitalPoint3D> OrbitalSampler::sample3D(const QuantumNumbers& qn,
        int numPoints,
        int Z,
        double samplingRadius) {
        std::vector<OrbitalPoint3D> points;
        points.reserve(numPoints);

        if (!qn.isValid() || numPoints <= 0) return points;

        const double maxProb = estimateMaxProbability3D(qn, Z, samplingRadius);
        if (maxProb <= 0.0) return points;

        std::uniform_real_distribution<double> uni(-samplingRadius, samplingRadius);
        std::uniform_real_distribution<double> uprob(0.0, maxProb);
        const int maxAttempts = numPoints * 5000;
        int attempts = 0;

        while (static_cast<int>(points.size()) < numPoints && attempts < maxAttempts) {
            ++attempts;
            const double x = uni(rng_);
            const double y = uni(rng_);
            const double z = uni(rng_);
            const double p = probabilityDensity3D(qn, x, y, z, Z);

            if (uprob(rng_) < p) {
                OrbitalPoint3D pt;
                pt.x = static_cast<float>(x);
                pt.y = static_cast<float>(y);
                pt.z = static_cast<float>(z);
                pt.intensity = static_cast<float>(std::min(1.0, p / maxProb));
                points.push_back(pt);
            }
        }

        return points;
    }

}