// ============================================================
// ElementDatabase.cpp
// ============================================================

#include "physics/ElementDatabase.hpp"
#include "utils/FileIO.hpp"

#include <nlohmann/json.hpp>
#include <fmt/format.h>

#include <iostream>

namespace physics {

bool ElementDatabase::loadFromFile(const std::string& path) {
    std::string content;
    if (!utils::FileIO::readText(path, content)) {
        std::cerr << "[ElementDatabase] Falha ao abrir: " << path << "\n";
        return false;
    }

    try {
        auto j = nlohmann::json::parse(content);
        const auto& elems = j.at("elements");

        for (const auto& e : elems) {
            const int Z          = e.at("Z").get<int>();
            const std::string sy = e.at("symbol").get<std::string>();
            const std::string nm = e.at("name").get<std::string>();

            std::vector<ShellConfig> config;
            for (const auto& s : e.at("configuration")) {
                ShellConfig shell;
                shell.n         = s.at("n").get<int>();
                shell.l         = s.at("l").get<int>();
                shell.electrons = s.at("electrons").get<int>();
                config.push_back(shell);
            }

            auto element = std::make_shared<Element>(Z, sy, nm, std::move(config));
            list_.push_back(element);
            bySymbol_[sy] = element;
        }
    } catch (const std::exception& ex) {
        std::cerr << fmt::format("[ElementDatabase] Erro de parse: {}\n", ex.what());
        return false;
    }

    std::cout << fmt::format("[ElementDatabase] Carregados {} elementos\n", list_.size());
    return true;
}

std::shared_ptr<Element> ElementDatabase::get(const std::string& symbol) const {
    auto it = bySymbol_.find(symbol);
    return (it != bySymbol_.end()) ? it->second : nullptr;
}

} // namespace physics
