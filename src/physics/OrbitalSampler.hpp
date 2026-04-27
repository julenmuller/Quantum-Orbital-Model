// ============================================================
// OrbitalSampler.hpp
// ------------------------------------------------------------
// Gera um conjunto de pontos (x, z) no plano 2D onde cada ponto
// representa uma "posição possível do elétron", amostrada
// proporcionalmente à densidade de probabilidade |ψ|².
//
// Método usado: Rejection Sampling
//   1. Sorteia um ponto aleatório dentro de um círculo grande.
//   2. Calcula |ψ|² nesse ponto.
//   3. Sorteia u ∈ [0, maxProb]. Aceita se u < |ψ|².
//   4. Repete até ter N pontos.
//
// Cada ponto aceito carrega também sua "intensidade" (valor
// de |ψ|² ali), usada pela renderização para colorir.
// ============================================================

#pragma once

#include "physics/QuantumNumbers.hpp"

#include <vector>
#include <random>

namespace physics {

struct OrbitalPoint {
    float x;        // coordenada x (unidades de raio de Bohr)
    float z;        // coordenada z (unidades de raio de Bohr)
    float intensity; // valor normalizado de |ψ|² no ponto (0..1)
};

class OrbitalSampler {
public:
    OrbitalSampler();

    // Gera 'numPoints' pontos amostrados para o orbital (n,l,m) de Z.
    // 'samplingRadius' é o raio da região onde fazemos o sorteio
    // (em unidades de Bohr). Tipicamente ~ 30 para n=3.
    std::vector<OrbitalPoint> sample(const QuantumNumbers& qn,
                                      int numPoints,
                                      int Z = 1,
                                      double samplingRadius = 30.0);

private:
    // Descobre o valor máximo aproximado de |ψ|² na região amostrada,
    // pra calibrar o rejection sampling.
    double estimateMaxProbability(const QuantumNumbers& qn,
                                   int Z,
                                   double samplingRadius);

    std::mt19937 rng_;  // gerador de números aleatórios
};

} // namespace physics
