// ============================================================
// Wavefunction.hpp
// ------------------------------------------------------------
// Combina parte radial (R_nl) e angular (Y_lm) para formar
// a função de onda completa ψ_nlm(r, θ, φ).
//
// A densidade de probabilidade |ψ|² é o que visualizamos
// na tela (áreas mais brilhantes = maior probabilidade
// de encontrar o elétron ali).
//
// Para um corte 2D (plano xz), usamos φ = 0.
// ============================================================

#pragma once

#include "physics/QuantumNumbers.hpp"

namespace physics {

class Wavefunction {
public:
    // Valor de ψ(r, θ, φ) para um orbital (n,l,m) de um núcleo
    // com número atômico Z.
    static double evaluate(const QuantumNumbers& qn,
                           double r, double theta, double phi,
                           int Z = 1);

    // Densidade de probabilidade |ψ|²
    static double probabilityDensity(const QuantumNumbers& qn,
                                      double r, double theta, double phi,
                                      int Z = 1);

    // ----- Versões em coordenadas cartesianas 2D -----
    // Conveniente para amostragem num plano (x, z).
    // Aqui φ = 0 (plano xz: y = 0).
    static double evaluate2D(const QuantumNumbers& qn,
                             double x, double z, int Z = 1);

    static double probabilityDensity2D(const QuantumNumbers& qn,
                                        double x, double z, int Z = 1);
};

} // namespace physics
