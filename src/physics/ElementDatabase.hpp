// ============================================================
// ElementDatabase.hpp
// ------------------------------------------------------------
// Catálogo de elementos disponíveis, carregado de assets/data/
// elements.json. Você adiciona novos elementos editando o JSON,
// sem recompilar.
//
// Exemplo do formato JSON:
//   {
//     "elements": [
//       { "Z": 1, "symbol": "H", "name": "Hidrogênio",
//         "configuration": [ { "n": 1, "l": 0, "electrons": 1 } ] },
//       ...
//     ]
//   }
// ============================================================

#pragma once

#include "physics/Element.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace physics {

class ElementDatabase {
public:
    // Carrega do JSON. Retorna false em caso de erro (arquivo faltando, etc.)
    bool loadFromFile(const std::string& path);

    // Lookup por símbolo ("H", "O", ...)
    std::shared_ptr<Element> get(const std::string& symbol) const;

    // Lista de todos os elementos disponíveis (útil pra UI)
    const std::vector<std::shared_ptr<Element>>& all() const { return list_; }

private:
    std::vector<std::shared_ptr<Element>> list_;
    std::unordered_map<std::string, std::shared_ptr<Element>> bySymbol_;
};

} // namespace physics
