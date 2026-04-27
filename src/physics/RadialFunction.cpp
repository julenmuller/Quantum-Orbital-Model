// ============================================================
// RadialFunction.cpp
// ============================================================

#include "physics/RadialFunction.hpp"

#include <cmath>
#include <array>

namespace physics {

// Cache de fatoriais (até 20!, mais que suficiente para n <= 5)
double RadialFunction::factorial(int n) {
    static const std::array<double, 21> table = [] {
        std::array<double, 21> t{};
        t[0] = 1.0;
        for (int i = 1; i <= 20; ++i) {
            t[i] = t[i - 1] * i;
        }
        return t;
    }();

    if (n < 0) return 0.0;
    if (n > 20) {
        // Fallback via recursão (não deve acontecer no uso normal)
        double result = table[20];
        for (int i = 21; i <= n; ++i) result *= i;
        return result;
    }
    return table[n];
}

// Polinômio associado de Laguerre L^α_k(x)
// Usa a relação de recorrência:
//   L^α_0(x) = 1
//   L^α_1(x) = 1 + α - x
//   (k+1) * L^α_(k+1)(x) = (2k+1+α-x) * L^α_k(x) - (k+α) * L^α_(k-1)(x)
double RadialFunction::laguerre(int k, int alpha, double x) {
    if (k == 0) return 1.0;
    if (k == 1) return 1.0 + alpha - x;

    double Lprev = 1.0;                       // L^α_0
    double Lcurr = 1.0 + alpha - x;           // L^α_1

    for (int i = 1; i < k; ++i) {
        double Lnext = ((2.0 * i + 1.0 + alpha - x) * Lcurr
                        - (i + alpha) * Lprev) / (i + 1.0);
        Lprev = Lcurr;
        Lcurr = Lnext;
    }
    return Lcurr;
}

double RadialFunction::evaluate(int n, int l, double r, int Z) {
    // Guardas de validação
    if (n <= 0 || l < 0 || l >= n) return 0.0;
    if (r < 0.0) return 0.0;

    // ρ = 2Zr / (n·a₀), com a₀ = 1 (unidades atômicas)
    const double rho = 2.0 * Z * r / n;

    // Constante de normalização
    // sqrt( (2Z/n)³ · (n-l-1)! / (2n·(n+l)!) )
    const double zOverN3 = std::pow(2.0 * Z / n, 3);
    const double numerator = factorial(n - l - 1);
    const double denominator = 2.0 * n * factorial(n + l);
    const double norm = std::sqrt(zOverN3 * numerator / denominator);

    // Termo exponencial e polinomial
    const double expTerm = std::exp(-rho / 2.0);
    const double rhoL = std::pow(rho, l);
    const double laguerreTerm = laguerre(n - l - 1, 2 * l + 1, rho);

    return norm * expTerm * rhoL * laguerreTerm;
}

} // namespace physics
