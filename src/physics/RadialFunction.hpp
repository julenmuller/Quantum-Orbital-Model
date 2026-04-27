// ============================================================
// RadialFunction.hpp
// ------------------------------------------------------------
// A função de onda ψ_nlm(r, θ, φ) de um orbital hidrogenoide
// pode ser separada em duas partes:
//   ψ(r, θ, φ) = R_nl(r) * Y_lm(θ, φ)
// Onde:
//   R_nl(r)   é a parte radial (só depende de r e dos números n, l)
//   Y_lm(θ,φ) é a parte angular (harmônicos esféricos)
//
// Esta classe calcula R_nl(r) usando polinômios associados
// de Laguerre, que são a solução canônica da equação de
// Schrödinger para átomos hidrogenoides.
//
// Fórmula:
//   R_nl(r) = sqrt( (2Z/(n·a₀))³ · (n-l-1)! / (2n·(n+l)!) )
//             · exp(-ρ/2) · ρ^l · L^(2l+1)_(n-l-1)(ρ)
// Onde:
//   ρ = 2Zr / (n·a₀)
//   a₀ = raio de Bohr (usamos unidades atômicas: a₀ = 1)
//   Z = número atômico do núcleo
//   L^α_k = polinômio associado de Laguerre
// ============================================================

#pragma once

namespace physics {

class RadialFunction {
public:
    // Calcula R_nl(r) para um dado raio r (em unidades de Bohr).
    // Z = número atômico (1 para H, 8 para O, etc.).
    //
    // Nota importante: para átomos multieletrônicos como O,
    // essa é uma APROXIMAÇÃO (trata cada elétron como se fosse
    // o único, sem repulsão elétron-elétron). Pra fins educativos
    // visuais, é suficiente.
    static double evaluate(int n, int l, double r, int Z = 1);

private:
    // Polinômio associado de Laguerre L^α_k(x)
    // k = grau, alpha = parâmetro, x = argumento
    static double laguerre(int k, int alpha, double x);

    // Fatorial (guardamos em cache pra não recalcular)
    static double factorial(int n);
};

} // namespace physics
