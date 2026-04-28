#include "app/Application.hpp"
#include <iostream>
#include <memory>

int main() {
    try {
        auto application = std::make_unique<app::Application>();
        const int initCode = application->initialize();
        if (initCode != 0) {
            std::cerr << "Falha na inicializacao. Codigo: " << initCode << "\n";
            return initCode;
        }
        application->run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Excecao: " << ex.what() << "\n";
        return 99;
    }
    catch (...) {
        std::cerr << "Excecao desconhecida\n";
        return 100;
    }
    return 0;
}