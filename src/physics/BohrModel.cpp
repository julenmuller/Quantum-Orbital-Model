// ============================================================
// BohrModel.cpp
// ============================================================

#include "physics/BohrModel.hpp"

namespace physics {

double BohrModel::shellRadius(int n, int Z) {
    if (n < 1 || Z < 1) return 0.0;
    return static_cast<double>(n * n) / Z;
}

double BohrModel::energyLevel(int n, int Z) {
    if (n < 1 || Z < 1) return 0.0;
    constexpr double RYDBERG_EV = 13.605693;
    return -RYDBERG_EV * (Z * Z) / static_cast<double>(n * n);
}

} // namespace physics
