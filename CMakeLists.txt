# ============================================================
# CMakeLists.txt - Build do projeto Quantum Orbital Model
# ============================================================
# Organizado por camadas arquiteturais:
#   physics -> rendering -> ui -> app -> main
# ============================================================

cmake_minimum_required(VERSION 3.20)

project(QuantumOrbitalModel
    VERSION 0.1.0
    DESCRIPTION "Visualizador 2D de orbitais quânticos"
    LANGUAGES CXX
)

# ----- Configurações gerais de C++ -----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)           # usa C++17 padrão, não extensões

# Coloca binário direto em build/ (facilita rodar sem achar em subpastas)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# ----- Dependências (via vcpkg) -----
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# ----- Todos os arquivos-fonte do projeto -----
set(PROJECT_SOURCES
    # Entry point
    src/main.cpp

    # Application (orquestração)
    src/app/Application.cpp

    # Platform (janela, input)
    src/platform/Window.cpp
    src/platform/Input.cpp

    # Physics (modelo matemático do átomo)
    src/physics/Wavefunction.cpp
    src/physics/RadialFunction.cpp
    src/physics/SphericalHarmonics.cpp
    src/physics/OrbitalSampler.cpp
    src/physics/Orbital.cpp
    src/physics/Element.cpp
    src/physics/ElementDatabase.cpp
    src/physics/BohrModel.cpp

    # Rendering (OpenGL moderno)
    src/rendering/Shader.cpp
    src/rendering/VertexBuffer.cpp
    src/rendering/Camera2D.cpp
    src/rendering/ParticleRenderer.cpp
    src/rendering/OrbitRenderer.cpp

    # UI (Dear ImGui)
    src/ui/UIManager.cpp
    src/ui/ControlPanel.cpp
    src/ui/InfoPanel.cpp

    # Utilities
    src/utils/FileIO.cpp
)

# ----- Define o executável -----
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# ----- Includes (permite #include "app/X.hpp" a partir de src/) -----
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ----- Linka todas as bibliotecas -----
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
    imgui::imgui
    nlohmann_json::nlohmann_json
    fmt::fmt
)

# ----- Flags de aviso agressivas (código limpo) -----
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

# ----- Copia assets/ para junto do executável na build -----
# Assim o programa acha elements.json, shaders, etc. em tempo de execução.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copiando assets/ para o diretório do executável"
)

# ----- Mensagens informativas no final da configuração -----
message(STATUS "=====================================================")
message(STATUS "  ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "=====================================================")
