// ============================================================
// BohrModel.hpp
// ------------------------------------------------------------
// Modelo de Bohr clássico: representação dos NÍVEIS DE ENERGIA
// como camadas circulares concêntricas (anéis de raios fixos).
//
// Esse modelo é fisicamente incorreto (átomos não são planetas),
// mas é OTIMO didaticamente para mostrar o conceito de "camadas".
//
// Uso na nossa visualização: desenhamos os anéis de Bohr POR CIMA
// da nuvem de probabilidade quântica para que o usuário consiga
// associar visualmente o nível n=1, n=2, etc.
//
// Raio de Bohr para o n-ésimo nível:
//   r_n = n² · a₀ / Z
// Onde a₀ é o raio de Bohr (= 1 em unidades atômicas).
// ============================================================

#pragma once

namespace physics {

class BohrModel {
public:
    // Calcula o raio (em unidades de Bohr) da n-ésima camada
    // para um núcleo com número atômico Z.
    static double shellRadius(int n, int Z = 1);

    // Energia do n-ésimo nível (em elétron-volts).
    //   E_n = -13.6 eV · Z² / n²
    static double energyLevel(int n, int Z = 1);
};

} // namespace physics
