// ============================================================
// VertexBuffer.cpp - LAZY INIT (cria VAO/VBO na primeira upload)
// ============================================================

#include "rendering/VertexBuffer.hpp"

#include <GL/glew.h>

namespace rendering {

VertexBuffer::VertexBuffer() {
    // Construtor vazio: NAO criamos VAO/VBO aqui porque
    // o contexto OpenGL pode nao existir ainda.
}

VertexBuffer::~VertexBuffer() {
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

void VertexBuffer::upload(const float* data, size_t floatCount,
                           const std::vector<VertexAttribute>& attributes,
                           bool dynamic) {
    // Lazy init: cria VAO/VBO so na primeira chamada
    if (vao_ == 0) {
        glGenVertexArrays(1, &vao_);
    }
    if (vbo_ == 0) {
        glGenBuffers(1, &vbo_);
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    const GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    glBufferData(GL_ARRAY_BUFFER,
                 floatCount * sizeof(float),
                 data, usage);

    int strideFloats = 0;
    for (const auto& a : attributes) strideFloats += a.components;
    strideFloats_ = strideFloats;
    vertexCount_ = (strideFloats > 0)
                   ? static_cast<int>(floatCount / strideFloats)
                   : 0;

    int offset = 0;
    for (const auto& a : attributes) {
        glEnableVertexAttribArray(a.location);
        glVertexAttribPointer(
            a.location,
            a.components,
            GL_FLOAT,
            a.normalized ? GL_TRUE : GL_FALSE,
            strideFloats * sizeof(float),
            reinterpret_cast<void*>(offset * sizeof(float))
        );
        offset += a.components;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexBuffer::bind() const {
    glBindVertexArray(vao_);
}

void VertexBuffer::unbind() {
    glBindVertexArray(0);
}

} // namespace rendering
