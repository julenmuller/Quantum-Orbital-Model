// ============================================================
// VertexBuffer.hpp
// ------------------------------------------------------------
// Encapsula um VAO + VBO (e opcionalmente EBO) do OpenGL.
// Permite enviar dados de vértices pra GPU e configurar o
// layout dos atributos.
// ============================================================

#pragma once

#include <vector>

namespace rendering {

// Descreve um atributo (ex.: posição = 2 floats, cor = 3 floats)
struct VertexAttribute {
    int location;       // location no shader (0, 1, 2...)
    int components;     // 1, 2, 3 ou 4
    bool normalized;
};

class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    // Carrega dados (em floats) e configura layout.
    // 'data' é um array contíguo: [v0_attr0, v0_attr1, ..., v1_attr0, ...]
    // 'attributes' descreve cada coluna na ordem.
    void upload(const float* data, size_t floatCount,
                const std::vector<VertexAttribute>& attributes,
                bool dynamic = false);

    void bind() const;
    static void unbind();

    int vertexCount() const { return vertexCount_; }

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    int vertexCount_ = 0;
    int strideFloats_ = 0;
};

} // namespace rendering
