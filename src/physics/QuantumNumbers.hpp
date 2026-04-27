// ============================================================
// QuantumNumbers.hpp
// ------------------------------------------------------------
// Representa os três números quânticos que definem um orbital:
//   n - número quântico principal (1, 2, 3, ...)
//       Determina o tamanho/energia do orbital (a "camada").
//   l - número quântico azimutal (0 <= l < n)
//       Determina a forma do orbital (s=0, p=1, d=2, f=3).
//   m - número quântico magnético (-l <= m <= +l)
//       Determina a orientação espacial do orbital.
//
// Regras físicas:
//   - Para cada n, l pode ir de 0 até n-1.
//   - Para cada l, m pode ir de -l até +l.
//   - Exemplo: n=2 permite l=0 (orbital 2s) e l=1 (orbital 2p).
//              l=1 permite m=-1, 0, +1 (três orbitais 2p).
// ============================================================

#pragma once

#include <string>

namespace physics {

struct QuantumNumbers {
    int n = 1;  // principal    (>= 1)
    int l = 0;  // azimutal     (0..n-1)
    int m = 0;  // magnético    (-l..+l)

    // Verifica se a combinação é fisicamente válida
    bool isValid() const {
        if (n < 1) return false;
        if (l < 0 || l >= n) return false;
        if (m < -l || m > l) return false;
        return true;
    }

    // Nome espectroscópico: "1s", "2p", "3d", "4f", etc.
    // Útil pra mostrar na UI e nas descrições educativas.
    std::string spectroscopicName() const {
        static const char orbitalLetters[] = {'s', 'p', 'd', 'f', 'g', 'h'};
        std::string result = std::to_string(n);
        if (l >= 0 && l < static_cast<int>(sizeof(orbitalLetters))) {
            result += orbitalLetters[l];
        }
        return result;
    }
};

} // namespace physics
