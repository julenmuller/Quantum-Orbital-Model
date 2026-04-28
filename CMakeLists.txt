cmake_minimum_required(VERSION 3.20)

project(QuantumOrbitalModel
    VERSION 0.2.0
    DESCRIPTION "Visualizador 2D/3D de orbitais quânticos"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Dependências
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Arquivos
set(PROJECT_SOURCES
    # Entry point
    src/main.cpp

    # Application
    src/app/Application.cpp

    # Platform
    src/platform/Window.cpp
    src/platform/Input.cpp

    # Physics
    src/physics/Wavefunction.cpp
    src/physics/RadialFunction.cpp
    src/physics/SphericalHarmonics.cpp
    src/physics/OrbitalSampler.cpp
    src/physics/Orbital.cpp
    src/physics/Element.cpp
    src/physics/ElementDatabase.cpp
    src/physics/BohrModel.cpp

    # Rendering
    src/rendering/Shader.cpp
    src/rendering/VertexBuffer.cpp
    src/rendering/Camera2D.cpp
    src/rendering/Camera3D.cpp
    src/rendering/ParticleRenderer.cpp
    src/rendering/OrbitRenderer.cpp

    # UI
    src/ui/UIManager.cpp
    src/ui/ControlPanel.cpp
    src/ui/InfoPanel.cpp

    # Utils
    src/utils/FileIO.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
    imgui::imgui
    nlohmann_json::nlohmann_json
    fmt::fmt
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

# Copia assets pra junto do executável
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copiando assets/ para o diretório do executável"
)

message(STATUS "=====================================================")
message(STATUS "  ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "=====================================================")