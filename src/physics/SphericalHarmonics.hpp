// ============================================================
// SphericalHarmonics.hpp
// ------------------------------------------------------------
// Os harmônicos esféricos Y_lm(θ, φ) são a parte angular da
// função de onda. Eles descrevem COMO o orbital se orienta
// no espaço (a forma característica de s, p, d, f).
//
// Para fins de VISUALIZAÇÃO (não cálculo de energia), usamos
// os harmônicos esféricos REAIS (combinações lineares dos
// complexos) porque são diretamente visualizáveis:
//   p_x, p_y, p_z em vez de p_(+1), p_(-1), p_0
//
// No nosso caso 2D, fixamos φ = 0 (plano xz) ou φ = π/2 (plano yz)
// e varremos θ, gerando um corte 2D do orbital.
// ============================================================

#pragma once

namespace physics {

class SphericalHarmonics {
public:
    // Calcula o harmônico esférico REAL Y_lm(θ, φ).
    //
    // Convenção de coordenadas esféricas:
    //   θ ∈ [0, π]   - ângulo polar (a partir do eixo z)
    //   φ ∈ [0, 2π]  - ângulo azimutal (no plano xy)
    //
    // Retorna um número real (não complexo).
    static double evaluate(int l, int m, double theta, double phi);

private:
    // Polinômio associado de Legendre P_l^m(x)
    static double legendre(int l, int m, double x);

    // Fatorial com cache
    static double factorial(int n);
};

} // namespace physics
