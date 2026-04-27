// ============================================================
// SphericalHarmonics.cpp
// ============================================================

#include "physics/SphericalHarmonics.hpp"

#include <cmath>
#include <array>

namespace physics {

namespace {
    constexpr double PI = 3.14159265358979323846;
}

double SphericalHarmonics::factorial(int n) {
    static const std::array<double, 21> table = [] {
        std::array<double, 21> t{};
        t[0] = 1.0;
        for (int i = 1; i <= 20; ++i) t[i] = t[i - 1] * i;
        return t;
    }();
    if (n < 0) return 0.0;
    if (n > 20) {
        double result = table[20];
        for (int i = 21; i <= n; ++i) result *= i;
        return result;
    }
    return table[n];
}

// Polinômio associado de Legendre P_l^m(x) — recorrência estável
// x tipicamente = cos(θ)
double SphericalHarmonics::legendre(int l, int m, double x) {
    const int absM = std::abs(m);
    if (absM > l) return 0.0;

    // Começa com P_m^m usando fórmula fechada
    double pmm = 1.0;
    if (absM > 0) {
        const double somx2 = std::sqrt((1.0 - x) * (1.0 + x));
        double fact = 1.0;
        for (int i = 1; i <= absM; ++i) {
            pmm *= -fact * somx2;
            fact += 2.0;
        }
    }

    if (l == absM) return pmm;

    // P_(m+1)^m
    double pmmp1 = x * (2.0 * absM + 1.0) * pmm;
    if (l == absM + 1) return pmmp1;

    // Recorrência para l > m+1:
    //   (l - m) * P_l^m = x(2l-1) * P_(l-1)^m - (l+m-1) * P_(l-2)^m
    double pll = 0.0;
    for (int ll = absM + 2; ll <= l; ++ll) {
        pll = (x * (2.0 * ll - 1.0) * pmmp1 - (ll + absM - 1.0) * pmm)
              / (ll - absM);
        pmm = pmmp1;
        pmmp1 = pll;
    }
    return pll;
}

double SphericalHarmonics::evaluate(int l, int m, double theta, double phi) {
    if (l < 0 || std::abs(m) > l) return 0.0;

    // Fator de normalização N_lm
    const double num = (2.0 * l + 1.0) * factorial(l - std::abs(m));
    const double den = 4.0 * PI * factorial(l + std::abs(m));
    const double norm = std::sqrt(num / den);

    // Parte de Legendre (depende só de θ)
    const double legendreVal = legendre(l, std::abs(m), std::cos(theta));

    // Harmônico esférico REAL:
    //   m > 0:  sqrt(2) * N_lm * P_l^|m|(cos θ) * cos(|m|·φ)
    //   m < 0:  sqrt(2) * N_lm * P_l^|m|(cos θ) * sin(|m|·φ)
    //   m = 0:  N_l0 * P_l^0(cos θ)
    if (m > 0) {
        return std::sqrt(2.0) * norm * legendreVal * std::cos(m * phi);
    } else if (m < 0) {
        return std::sqrt(2.0) * norm * legendreVal * std::sin(-m * phi);
    } else {
        return norm * legendreVal;
    }
}

} // namespace physics
