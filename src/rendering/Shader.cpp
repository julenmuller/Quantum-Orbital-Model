// ============================================================
// Shader.cpp
// ============================================================

#include "rendering/Shader.hpp"
#include "utils/FileIO.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>

#include <iostream>
#include <vector>

namespace rendering {

Shader::Shader() = default;

Shader::~Shader() {
    if (program_) glDeleteProgram(program_);
}

unsigned int Shader::compileStage(unsigned int type,
                                   const std::string& source,
                                   const std::string& tag) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << fmt::format("[Shader/{}] Erro de compilação:\n{}\n",
                                  tag, log.data());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Shader::loadFromFiles(const std::string& vertPath,
                            const std::string& fragPath) {
    std::string vertSrc, fragSrc;
    if (!utils::FileIO::readText(vertPath, vertSrc)) {
        std::cerr << "[Shader] Falha ao abrir: " << vertPath << "\n";
        return false;
    }
    if (!utils::FileIO::readText(fragPath, fragSrc)) {
        std::cerr << "[Shader] Falha ao abrir: " << fragPath << "\n";
        return false;
    }

    unsigned int vert = compileStage(GL_VERTEX_SHADER, vertSrc, "vertex");
    if (!vert) return false;
    unsigned int frag = compileStage(GL_FRAGMENT_SHADER, fragSrc, "fragment");
    if (!frag) {
        glDeleteShader(vert);
        return false;
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vert);
    glAttachShader(program_, frag);
    glLinkProgram(program_);

    int linked = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &linked);
    if (!linked) {
        int len = 0;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetProgramInfoLog(program_, len, nullptr, log.data());
        std::cerr << fmt::format("[Shader] Erro de link:\n{}\n", log.data());
        glDeleteShader(vert);
        glDeleteShader(frag);
        glDeleteProgram(program_);
        program_ = 0;
        return false;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return true;
}

void Shader::use() const {
    glUseProgram(program_);
}

int Shader::getUniformLocation(const std::string& name) {
    auto it = uniformCache_.find(name);
    if (it != uniformCache_.end()) return it->second;
    int loc = glGetUniformLocation(program_, name.c_str());
    uniformCache_[name] = loc;
    return loc;
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

} // namespace rendering
