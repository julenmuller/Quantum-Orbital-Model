// ============================================================
// InfoPanel.hpp
// ------------------------------------------------------------
// Painel educativo: mostra informações físicas sobre o
// elemento atual (configuração eletrônica, energia por nível,
// descrição de cada orbital).
// ============================================================

#pragma once

#include "physics/Element.hpp"
#include "physics/Orbital.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ui {

class InfoPanel {
public:
    // Carrega as descrições educativas a partir de
    // assets/data/orbital_descriptions.json
    bool loadDescriptions(const std::string& path);

    void render(const std::shared_ptr<physics::Element>& element,
                const std::vector<std::shared_ptr<physics::Orbital>>& orbitals);

private:
    // chave: "1s", "2p", etc.
    std::unordered_map<std::string, std::string> descriptions_;

    std::string descriptionFor(const std::string& orbitalName) const;
};

} // namespace ui
