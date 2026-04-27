// ============================================================
// Element.hpp
// ------------------------------------------------------------
// Representa um elemento químico (H, O, He, ...).
// Contém:
//   - Número atômico (Z)
//   - Símbolo ("H", "O")
//   - Nome ("Hidrogênio", "Oxigênio")
//   - Configuração eletrônica: lista de (n,l,ocupação)
//
// A partir da configuração eletrônica, geramos todos os
// orbitais que devem ser visualizados para esse elemento.
// ============================================================

#pragma once

#include "physics/Orbital.hpp"
#include "physics/QuantumNumbers.hpp"

#include <memory>
#include <string>
#include <vector>

namespace physics {

// Uma "camada" da configuração eletrônica:
//   Exemplo: orbital 2p com 4 elétrons -> { n=2, l=1, electrons=4 }
struct ShellConfig {
    int n;
    int l;
    int electrons;
};

class Element {
public:
    Element(int Z, std::string symbol, std::string name,
            std::vector<ShellConfig> config);

    int atomicNumber() const { return Z_; }
    const std::string& symbol() const { return symbol_; }
    const std::string& name() const { return name_; }
    const std::vector<ShellConfig>& configuration() const { return config_; }

    // Gera todos os orbitais (um por combinação n,l,m permitida).
    // Para um (n,l) com elétrons, criamos orbitais para m = -l..+l.
    std::vector<std::shared_ptr<Orbital>> createOrbitals() const;

private:
    int Z_;
    std::string symbol_;
    std::string name_;
    std::vector<ShellConfig> config_;
};

} // namespace physics
