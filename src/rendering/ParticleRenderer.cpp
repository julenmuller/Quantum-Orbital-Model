// ============================================================
// ParticleRenderer.cpp - VERSAO DEBUG
// ============================================================

#include "rendering/ParticleRenderer.hpp"

#include <GL/glew.h>

#include <iostream>

namespace rendering {

ParticleRenderer::ParticleRenderer() = default;

bool ParticleRenderer::initialize(const std::string& shaderDir) {
    bool ok = shader_.loadFromFiles(
        shaderDir + "/particle.vert",
        shaderDir + "/particle.frag"
    );
    std::cout << "  [ParticleRenderer] Shader load: " << (ok ? "OK" : "FAIL") << std::endl;
    std::cout.flush();
    return ok;
}

void ParticleRenderer::uploadOrbitals(
    const std::vector<std::shared_ptr<physics::Orbital>>& orbitals) {

    std::vector<float> data;
    totalPoints_ = 0;
    for (const auto& orb : orbitals) {
        for (const auto& p : orb->points()) {
            data.push_back(p.x);
            data.push_back(p.z);
            data.push_back(orb->color().r);
            data.push_back(orb->color().g);
            data.push_back(orb->color().b);
            data.push_back(p.intensity);
            ++totalPoints_;
        }
    }

    std::cout << "  [ParticleRenderer] Upload: " << totalPoints_ << " pontos, "
              << data.size() << " floats" << std::endl;
    std::cout.flush();

    if (totalPoints_ == 0) return;

    // Print os primeiros 3 pontos pra debug
    for (int i = 0; i < std::min(3, totalPoints_); ++i) {
        int base = i * 6;
        std::cout << "  [ParticleRenderer] Ponto " << i << ": ("
                  << data[base] << ", " << data[base+1] << ") cor=("
                  << data[base+2] << ", " << data[base+3] << ", " << data[base+4]
                  << ") I=" << data[base+5] << std::endl;
    }
    std::cout.flush();

    buffer_.upload(data.data(), data.size(), {
        {0, 2, false},
        {1, 3, false},
        {2, 1, false}
    });
}

void ParticleRenderer::render(const Camera2D& camera, float pointSize) {
    if (totalPoints_ == 0) return;

    // GARANTE estado OpenGL correto
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    shader_.use();
    shader_.setMat4("uProjection", camera.projectionMatrix());
    shader_.setMat4("uView", camera.viewMatrix());
    shader_.setFloat("uPointSize", pointSize);

    buffer_.bind();
    glDrawArrays(GL_POINTS, 0, totalPoints_);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "  [ParticleRenderer] GL Error: 0x" << std::hex << err << std::dec << std::endl;
        std::cerr.flush();
    }

    VertexBuffer::unbind();
}

} // namespace rendering
