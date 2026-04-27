// ============================================================
// Orbital.hpp
// ------------------------------------------------------------
// Representa UM orbital específico (n, l, m) de um elemento.
// Guarda os pontos amostrados (cache) para não recalcular
// toda vez que vai renderizar.
//
// Também armazena metadados: cor sugerida, número de elétrons
// ocupando esse orbital, descrição educativa.
// ============================================================

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

    // Gera os pontos amostrados (chamar uma vez por orbital).
    void generatePoints(OrbitalSampler& sampler, int numPoints,
                        double samplingRadius = 30.0);

    // Acesso aos pontos cacheados
    const std::vector<OrbitalPoint>& points() const { return points_; }

    const QuantumNumbers& quantumNumbers() const { return qn_; }
    int atomicNumber() const { return Z_; }

    // Cor e ocupação (setadas pelo Element)
    void setColor(const glm::vec3& color) { color_ = color; }
    const glm::vec3& color() const { return color_; }

    void setOccupancy(int electrons) { occupancy_ = electrons; }
    int occupancy() const { return occupancy_; }

    // Nome espectroscópico (delegado aos QN)
    std::string name() const { return qn_.spectroscopicName(); }

private:
    QuantumNumbers qn_;
    int Z_;
    std::vector<OrbitalPoint> points_;
    glm::vec3 color_{1.0f, 1.0f, 1.0f};
    int occupancy_ = 0;  // quantos elétrons ocupam esse orbital
};

} // namespace physics
