#include "physics/Orbital.hpp"

namespace physics {

    Orbital::Orbital(const QuantumNumbers& qn, int Z)
        : qn_(qn), Z_(Z) {
    }

    void Orbital::generatePoints(OrbitalSampler& sampler, int numPoints,
        double samplingRadius) {
        points_ = sampler.sample(qn_, numPoints, Z_, samplingRadius);
    }

    void Orbital::generatePoints3D(OrbitalSampler& sampler, int numPoints,
        double samplingRadius) {
        points3D_ = sampler.sample3D(qn_, numPoints, Z_, samplingRadius);
    }

}