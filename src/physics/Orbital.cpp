// ============================================================
// Orbital.cpp
// ============================================================

#include "physics/Orbital.hpp"

namespace physics {

Orbital::Orbital(const QuantumNumbers& qn, int Z)
    : qn_(qn), Z_(Z) {}

void Orbital::generatePoints(OrbitalSampler& sampler, int numPoints,
                              double samplingRadius) {
    points_ = sampler.sample(qn_, numPoints, Z_, samplingRadius);
}

} // namespace physics
