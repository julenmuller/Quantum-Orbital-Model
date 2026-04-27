# Quantum Orbital Model — Visualizador 2D

Visualizador interativo 2D de orbitais atômicos com **física quântica real** (equação de Schrödinger), inspirado no projeto
(https://github.com/kavan010/Atoms) e completamente refatorado em uma arquitetura profissional por camadas.

<img width="1281" height="751" alt="Captura de tela 2026-04-27 171001" src="https://github.com/user-attachments/assets/66a5adfc-ddfa-4b42-8a87-8d81e426c676" />

[versão 2D]

## 🎯 O que ele faz

Mostra a **densidade de probabilidade** |ψ|² dos orbitais de um átomo em um corte 2D (plano *xz*), sobreposto às camadas de Bohr (anéis circulares). Cada ponto colorido representa uma posição possível do elétron, amostrada proporcionalmente à probabilidade.

### Recursos

- ✅ **Múltiplos elementos**: H, He, C, N, O (extensível por JSON — adicione novos sem recompilar)
- ✅ **Física real**: usa harmônicos esféricos + polinômios de Laguerre (solução exata da equação de Schrödinger para hidrogenoides)
- ✅ **Interface educativa**: painel com configuração eletrônica, energias por nível, descrição de cada orbital
- ✅ **Controles interativos**: ajuste número de partículas, tamanho dos pontos, zoom, ligar/desligar camadas de Bohr
- ✅ **OpenGL moderno**: shaders GLSL, VBO/VAO, rendering eficiente
- ✅ **Dear ImGui**: interface profissional e responsiva

---

## 📋 Pré-requisitos (configuração única)

Você já instalou tudo isso durante o setup:

- ✅ **Visual Studio 2022** com workload "Desktop development with C++" (Windows) ou `build-essential` (Linux)
- ✅ **CMake** ≥ 3.20
- ✅ **Git**
- ✅ **vcpkg** instalado, bootstrapado, e no PATH

---

## 🚀 Build e execução

### Passo 1: Clone ou copie o projeto

Se você está lendo isto, o projeto já está na sua máquina em `C:\Users\Meu Computador\Quantum-Orbital-Model`.

### Passo 2: Instale as dependências (via vcpkg)

Abra o **PowerShell** na pasta do projeto e rode:

```powershell
vcpkg install
```

Isso lê o `vcpkg.json` e instala automaticamente: `glew`, `glfw3`, `glm`, `imgui`, `nlohmann-json`, `fmt`.

**Tempo esperado:** 10–20 minutos (compila tudo do zero na primeira vez).

### Passo 3: Configure com CMake

```powershell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/Users/Meu Computador/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

> **Atenção:** Ajuste o caminho do vcpkg se estiver em outro lugar.

### Passo 4: Compile

```powershell
cmake --build build --config Release
```

**Tempo esperado:** 1–3 minutos.

### Passo 5: Execute

```powershell
.\build\Release\QuantumOrbitalModel.exe
```

Ou simplesmente:

```powershell
.\build\QuantumOrbitalModel.exe
```

(o caminho exato depende do gerador do CMake)

---

## 🎮 Como usar

### Interface

**Painel "Controles"** (canto superior esquerdo):
- **Elemento**: dropdown pra escolher H, He, C, N ou O
- **Pontos/orbital**: slider pra ajustar densidade da nuvem (500 a 50.000)
- **Botão "Regenerar nuvem"**: re-amostra os pontos (útil após mudar elemento ou densidade)
- **Mostrar camadas de Bohr**: liga/desliga os anéis circulares
- **Tamanho dos pontos**: visual
- **Zoom**: controla o quanto da cena cabe na tela

**Painel "Informações Físicas"** (abaixo do painel de controles):
- Símbolo e nome do elemento
- Número atômico Z
- Configuração eletrônica completa (ex.: 1s² 2s² 2p⁴)
- **Níveis de Energia**: energias em eV e raios em unidades de Bohr para cada camada *n*
- **Orbitais Ocupados**: descrição educativa de cada orbital (1s, 2p, etc.)

### Atalhos de teclado

- **ESC**: fecha o programa

---

## 📐 Arquitetura

Veja [docs/architecture.md](docs/architecture.md) para detalhes completos. Resumo da estrutura:

```
src/
├── main.cpp                     # entry point enxuto
├── app/                         # orquestração (Application, Config)
│   ├── Application.hpp/cpp      # ciclo de vida da aplicação
│   └── Config.hpp               # constantes globais
├── platform/                    # janela, input (GLFW)
│   ├── Window.hpp/cpp
│   └── Input.hpp/cpp
├── physics/                     # modelo físico (equação de Schrödinger)
│   ├── QuantumNumbers.hpp       # (n, l, m)
│   ├── RadialFunction.hpp/cpp   # R_nl(r) com Laguerre
│   ├── SphericalHarmonics.hpp/cpp # Y_lm(θ,φ) com Legendre
│   ├── Wavefunction.hpp/cpp     # ψ = R × Y
│   ├── OrbitalSampler.hpp/cpp   # rejection sampling de |ψ|²
│   ├── Orbital.hpp/cpp          # um orbital (n,l,m) + pontos
│   ├── Element.hpp/cpp          # elemento químico
│   ├── ElementDatabase.hpp/cpp  # catálogo carregado de JSON
│   └── BohrModel.hpp/cpp        # raios e energias das camadas
├── rendering/                   # OpenGL moderno
│   ├── Shader.hpp/cpp           # wrapper de shaders GLSL
│   ├── VertexBuffer.hpp/cpp     # VBO/VAO
│   ├── Camera2D.hpp/cpp         # projeção ortográfica 2D
│   ├── ParticleRenderer.hpp/cpp # desenha a nuvem de pontos
│   └── OrbitRenderer.hpp/cpp    # desenha os anéis de Bohr
├── ui/                          # Dear ImGui
│   ├── UIManager.hpp/cpp        # init/shutdown ImGui
│   ├── ControlPanel.hpp/cpp     # controles de elemento/amostragem
│   └── InfoPanel.hpp/cpp        # descrições educativas
└── utils/
    └── FileIO.hpp/cpp           # leitura de arquivos

assets/
├── data/
│   ├── elements.json            # H, He, C, N, O (adicione mais aqui!)
│   └── orbital_descriptions.json # textos educativos
└── shaders/
    ├── particle.vert            # vertex shader das partículas
    ├── particle.frag            # fragment shader das partículas
    ├── orbit.vert               # vertex shader dos anéis
    └── orbit.frag               # fragment shader dos anéis

scripts/
└── schrodinger_2d_validator.py  # validação da física em Python

docs/
├── architecture.md              # documento técnico de arquitetura
└── physics_notes.md             # notas sobre a física implementada
```

---

## ➕ Adicionando um novo elemento

Edite `assets/data/elements.json` (não precisa recompilar):

```json
{
  "Z": 9,
  "symbol": "F",
  "name": "Flúor",
  "configuration": [
    { "n": 1, "l": 0, "electrons": 2 },
    { "n": 2, "l": 0, "electrons": 2 },
    { "n": 2, "l": 1, "electrons": 5 }
  ]
}
```

Execute o programa de novo e "Flúor" aparecerá no dropdown.

---

## ⚠️ Limitações conhecidas

### Aproximação hidrogenoide

Para átomos com **mais de um elétron** (He, C, N, O), as funções de onda calculadas são **hidrogenoides**: tratam cada elétron como se fosse o único, ignorando repulsão elétron-elétron. Isso é:

- ✅ **Educativamente útil**: mostra a estrutura qualitativa dos orbitais (s esféricos, p em halter, etc.)
- ❌ **Fisicamente aproximado**: as energias e formas exatas requerem métodos de muitos corpos (Hartree-Fock, DFT)

**Em termos práticos:** a visualização está correta qualitativamente (você vê os lóbulos do 2p, as camadas do 1s/2s), mas os raios e densidades são aproximações.

### Visualização 2D

O que você vê é um **corte no plano *xz*** (com *y* = 0 fixo, φ = 0). Orbitais 3D reais têm estrutura nos três eixos. Para visualização 3D completa, veja o código original em [kavan010/Atoms](https://github.com/kavan010/Atoms) (especificamente `atom_realtime.cpp` e `atom_raytracer.cpp`).

### Sem rotação interativa dos lóbulos p

Cada orbital p é mostrado em sua orientação canônica (p_x ao longo de x, p_z ao longo de z). Combinações lineares (como as que formam ligações em moléculas) não são mostradas.

---

## 🔬 Física implementada

- **Números quânticos**: `(n, l, m)` descrevem cada orbital
- **Parte radial**: `R_nl(r)` calculada com polinômios associados de Laguerre
- **Parte angular**: `Y_lm(θ, φ)` calculada com harmônicos esféricos reais (Legendre)
- **Função de onda**: `ψ_nlm(r, θ, φ) = R_nl(r) · Y_lm(θ, φ)`
- **Densidade de probabilidade**: `|ψ|²`
- **Amostragem**: *rejection sampling* — sorteia pontos proporcionalmente a `|ψ|²`

Veja [docs/physics_notes.md](docs/physics_notes.md) para detalhes matemáticos.

---

## 🐍 Validação com Python

O script `scripts/schrodinger_2d_validator.py` calcula os mesmos orbitais em Python (NumPy + SciPy + Matplotlib) pra você comparar visualmente se o C++ está correto.

Rode:

```bash
python scripts/schrodinger_2d_validator.py
```

Ele gera plots 2D do orbital 2p do hidrogênio. Compare com o que aparece no programa C++.

---

## 🎓 Próximos passos (evolução do projeto)

### Curto prazo
- [ ] Adicionar mais elementos (Na, Mg, Al, ...)
- [ ] Descrições mais ricas no painel educativo (histórico, aplicações)
- [ ] Transições eletrônicas (animação de saltos entre níveis)

### Médio prazo
- [ ] Visualização 3D com rotação de câmera (já existe infraestrutura — só falta ativar)
- [ ] Shaders de volume rendering (em vez de pontos discretos)
- [ ] Comparação lado a lado: Bohr vs Schrödinger

### Longo prazo
- [ ] Método Hartree-Fock simplificado pra átomos multi-eletrônicos
- [ ] Moléculas simples (H₂, H₂O)
- [ ] Orbitais moleculares (LCAO)

---

## 📚 Referências

- **Projeto original**: [kavan010/Atoms](https://github.com/kavan010/Atoms)
- **Equação de Schrödinger**: Griffiths, *Introduction to Quantum Mechanics* (3rd ed.)
- **Harmônicos esféricos**: Arfken & Weber, *Mathematical Methods for Physicists*
- **Rendering**: LearnOpenGL (https://learnopengl.com)

---

## 📝 Licença

Código novo escrito do zero com arquitetura própria. Inspiração e conceito físico do projeto [kavan010/Atoms](https://github.com/kavan010/Atoms).

---

## 🤝 Contribuindo

Sugestões, correções e pull requests são bem-vindos! Abra uma issue ou fork o repositório.

---

**Desenvolvido com física quântica real e arquitetura de software profissional.** 🚀⚛️
