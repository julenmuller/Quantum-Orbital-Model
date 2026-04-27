// ============================================================
// main.cpp - VERSAO COM HEAP ALLOCATION
// ============================================================

#include "app/Application.hpp"

#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Quantum Orbital Model ===" << std::endl;
    std::cout.flush();

    int returnCode = 0;

    try {
        std::cout << "[main] Criando Application no heap..." << std::endl;
        std::cout.flush();

        auto application = std::make_unique<app::Application>();

        std::cout << "[main] Application criada. Chamando initialize..." << std::endl;
        std::cout.flush();

        const int initCode = application->initialize();
        if (initCode != 0) {
            std::cerr << "Falha na inicializacao. Codigo: " << initCode << "\n";
            returnCode = initCode;
        } else {
            std::cout << "[main] Inicializado! Entrando no loop..." << std::endl;
            std::cout.flush();
            application->run();
            std::cout << "[main] Programa encerrado." << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Excecao: " << ex.what() << "\n";
        returnCode = 99;
    } catch (...) {
        std::cerr << "Excecao desconhecida!\n";
        returnCode = 100;
    }

    std::cout << "\n--- Pressione Enter para fechar ---" << std::endl;
    std::cout.flush();
    std::cin.get();
    return returnCode;
}
