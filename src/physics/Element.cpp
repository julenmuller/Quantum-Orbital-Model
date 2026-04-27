// ============================================================
// Element.cpp
// ============================================================

#include "physics/Element.hpp"

namespace physics {

Element::Element(int Z, std::string symbol, std::string name,
                 std::vector<ShellConfig> config)
    : Z_(Z),
      symbol_(std::move(symbol)),
      name_(std::move(name)),
      config_(std::move(config))
{}

std::vector<std::shared_ptr<Orbital>> Element::createOrbitals() const {
    std::vector<std::shared_ptr<Orbital>> orbitals;

    for (const auto& shell : config_) {
        // Para cada (n, l), criamos 2l+1 orbitais (um por valor de m).
        // Distribuímos os elétrons entre eles conforme a regra de Hund
        // (simplificada: distribuição uniforme).
        const int numM = 2 * shell.l + 1;
        if (numM <= 0) continue;

        // Distribui elétrons: cada orbital pode ter 0, 1 ou 2 elétrons.
        // Regra de Hund: primeiro um em cada, depois pareia.
        std::vector<int> occupancies(numM, 0);
        int remaining = shell.electrons;

        // Primeiro passe: 1 elétron em cada m
        for (int i = 0; i < numM && remaining > 0; ++i) {
            occupancies[i] = 1;
            --remaining;
        }
        // Segundo passe: pareia
        for (int i = 0; i < numM && remaining > 0; ++i) {
            occupancies[i] = 2;
            --remaining;
        }

        int mIndex = 0;
        for (int m = -shell.l; m <= shell.l; ++m, ++mIndex) {
            if (occupancies[mIndex] == 0) continue;
            QuantumNumbers qn{shell.n, shell.l, m};
            auto orbital = std::make_shared<Orbital>(qn, Z_);
            orbital->setOccupancy(occupancies[mIndex]);
            orbitals.push_back(orbital);
        }
    }

    return orbitals;
}

} // namespace physics
