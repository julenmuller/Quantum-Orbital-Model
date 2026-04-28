#include "rendering/ParticleRenderer3D.hpp"
#include <GL/glew.h>
#include <iostream>

namespace rendering {

    ParticleRenderer3D::ParticleRenderer3D() = default;

    bool ParticleRenderer3D::initialize(const std::string& shaderDir) {
        bool ok = shader_.loadFromFiles(
            shaderDir + "/particle3d.vert",
            shaderDir + "/particle3d.frag"
        );
        std::cout << "  [ParticleRenderer3D] Shader load: " << (ok ? "OK" : "FAIL") << std::endl;
        std::cout.flush();
        return ok;
    }

    void ParticleRenderer3D::uploadOrbitals(
        const std::vector<std::shared_ptr<physics::Orbital>>& orbitals) {

        std::vector<float> data;
        totalPoints_ = 0;
        for (const auto& orb : orbitals) {
            for (const auto& p : orb->points3D()) {
                data.push_back(p.x);
                data.push_back(p.y);
                data.push_back(p.z);
                data.push_back(orb->color().r);
                data.push_back(orb->color().g);
                data.push_back(orb->color().b);
                data.push_back(p.intensity);
                ++totalPoints_;
            }
        }

        std::cout << "  [ParticleRenderer3D] Upload: " << totalPoints_ << " pontos 3D" << std::endl;
        std::cout.flush();

        if (totalPoints_ == 0) return;

        buffer_.upload(data.data(), data.size(), {
            {0, 3, false},
            {1, 3, false}, 
            {2, 1, false}   
            });
    }

    void ParticleRenderer3D::render(const Camera3D& camera, float pointSize) {
        if (totalPoints_ == 0) return;

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_PROGRAM_POINT_SIZE);

        shader_.use();
        shader_.setMat4("uProjection", camera.projectionMatrix());
        shader_.setMat4("uView", camera.viewMatrix());
        shader_.setFloat("uPointSize", pointSize);

        buffer_.bind();
        glDrawArrays(GL_POINTS, 0, totalPoints_);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "  [ParticleRenderer3D] GL Error: 0x"
                << std::hex << err << std::dec << std::endl;
            std::cerr.flush();
        }

        VertexBuffer::unbind();

        glDepthMask(GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

}