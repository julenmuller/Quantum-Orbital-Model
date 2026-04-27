// ============================================================
// InfoPanel.cpp
// ============================================================

#include "ui/InfoPanel.hpp"
#include "physics/BohrModel.hpp"
#include "utils/FileIO.hpp"

#include <imgui.h>
#include <nlohmann/json.hpp>
#include <fmt/format.h>

#include <iostream>
#include <set>

namespace ui {

bool InfoPanel::loadDescriptions(const std::string& path) {
    std::string content;
    if (!utils::FileIO::readText(path, content)) {
        std::cerr << "[InfoPanel] Falha ao abrir: " << path << "\n";
        return false;
    }
    try {
        auto j = nlohmann::json::parse(content);
        for (auto it = j.begin(); it != j.end(); ++it) {
            descriptions_[it.key()] = it.value().get<std::string>();
        }
    } catch (const std::exception& ex) {
        std::cerr << "[InfoPanel] Erro de parse: " << ex.what() << "\n";
        return false;
    }
    return true;
}

std::string InfoPanel::descriptionFor(const std::string& orbitalName) const {
    auto it = descriptions_.find(orbitalName);
    if (it != descriptions_.end()) return it->second;
    // Fallback: tenta só as letras (ex.: "2p" -> "p")
    if (orbitalName.size() >= 2) {
        std::string typeOnly = orbitalName.substr(orbitalName.size() - 1);
        auto it2 = descriptions_.find(typeOnly);
        if (it2 != descriptions_.end()) return it2->second;
    }
    return u8"(Sem descrição disponível)";
}

void InfoPanel::render(const std::shared_ptr<physics::Element>& element,
                        const std::vector<std::shared_ptr<physics::Orbital>>& orbitals) {
    if (!element) return;

    ImGui::SetNextWindowPos(ImVec2(10, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 380), ImGuiCond_FirstUseEver);
    ImGui::Begin(u8"Informações Físicas");

    // Cabeçalho do elemento
    ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.4f, 1.0f),
                        "%s (%s)", element->name().c_str(), element->symbol().c_str());
    ImGui::Text(u8"Número atômico Z = %d", element->atomicNumber());

    // Configuração eletrônica em texto
    std::string config;
    for (const auto& shell : element->configuration()) {
        physics::QuantumNumbers tempQn{shell.n, shell.l, 0};
        config += tempQn.spectroscopicName();
        config += "^" + std::to_string(shell.electrons) + " ";
    }
    ImGui::Text(u8"Configuração: %s", config.c_str());

    ImGui::Separator();

    // ----- Níveis de energia (Bohr) -----
    if (ImGui::CollapsingHeader(u8"Níveis de Energia (Modelo de Bohr)",
                                 ImGuiTreeNodeFlags_DefaultOpen)) {
        // Coleta os n únicos
        std::set<int> usedN;
        for (const auto& shell : element->configuration()) {
            usedN.insert(shell.n);
        }
        for (int n : usedN) {
            const double E = physics::BohrModel::energyLevel(n, element->atomicNumber());
            const double r = physics::BohrModel::shellRadius(n, element->atomicNumber());
            ImGui::BulletText(u8"n = %d  →  E = %.2f eV   r = %.2f a₀",
                               n, E, r);
        }
        ImGui::TextWrapped(u8"Energias mais negativas = elétron mais "
                            "fortemente ligado ao núcleo. O n=1 é o estado "
                            "fundamental (mais estável).");
    }

    // ----- Descrição de cada orbital ocupado -----
    if (ImGui::CollapsingHeader(u8"Orbitais Ocupados",
                                 ImGuiTreeNodeFlags_DefaultOpen)) {
        // Para evitar repetir a descrição quando há múltiplos m do mesmo l,
        // mostramos só a primeira ocorrência de cada (n,l).
        std::set<std::string> shown;
        for (const auto& orb : orbitals) {
            std::string name = orb->name();
            if (shown.count(name)) continue;
            shown.insert(name);

            ImGui::PushStyleColor(ImGuiCol_Text,
                ImVec4(orb->color().r, orb->color().g, orb->color().b, 1.0f));
            ImGui::Text(u8"● %s", name.c_str());
            ImGui::PopStyleColor();

            ImGui::TextWrapped("%s", descriptionFor(name).c_str());
            ImGui::Spacing();
        }
    }

    ImGui::End();
}

} // namespace ui
