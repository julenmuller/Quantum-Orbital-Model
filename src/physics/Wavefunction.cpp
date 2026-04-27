// ============================================================
// Wavefunction.cpp
// ============================================================

#include "physics/Wavefunction.hpp"
#include "physics/RadialFunction.hpp"
#include "physics/SphericalHarmonics.hpp"

#include <cmath>

namespace physics {

double Wavefunction::evaluate(const QuantumNumbers& qn,
                              double r, double theta, double phi,
                              int Z) {
    const double R = RadialFunction::evaluate(qn.n, qn.l, r, Z);
    const double Y = SphericalHarmonics::evaluate(qn.l, qn.m, theta, phi);
    return R * Y;
}

double Wavefunction::probabilityDensity(const QuantumNumbers& qn,
                                         double r, double theta, double phi,
                                         int Z) {
    const double psi = evaluate(qn, r, theta, phi, Z);
    return psi * psi;
}

double Wavefunction::evaluate2D(const QuantumNumbers& qn,
                                double x, double z, int Z) {
    // Converte (x, z) para (r, θ) no plano xz (y=0, φ=0).
    const double r = std::sqrt(x * x + z * z);
    if (r < 1e-12) return 0.0;
    // θ é medido a partir do eixo z (convenção física padrão)
    const double theta = std::acos(z / r);
    return evaluate(qn, r, theta, 0.0, Z);
}

double Wavefunction::probabilityDensity2D(const QuantumNumbers& qn,
                                           double x, double z, int Z) {
    const double psi = evaluate2D(qn, x, z, Z);
    return psi * psi;
}

} // namespace physics
