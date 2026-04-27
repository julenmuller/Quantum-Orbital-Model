// ============================================================
// FileIO.hpp
// ------------------------------------------------------------
// Utilitários simples de leitura de arquivos. Centraliza
// o tratamento de erros para não espalhar ifstream pelo código.
// ============================================================

#pragma once

#include <string>

namespace utils {

class FileIO {
public:
    // Lê todo o conteúdo de um arquivo de texto em 'out'.
    // Retorna false se não conseguir abrir.
    static bool readText(const std::string& path, std::string& out);
};

} // namespace utils
