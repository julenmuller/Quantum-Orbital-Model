// ============================================================
// Shader.hpp
// ------------------------------------------------------------
// Wrapper de programa GLSL: carrega vertex+fragment shader,
// compila, linka, e oferece API pra setar uniforms.
// ============================================================

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace rendering {

class Shader {
public:
    Shader();
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Carrega e compila a partir de arquivos.
    // Retorna false em caso de erro (mensagens vão pro stderr).
    bool loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    void use() const;

    // Setters de uniforms
    void setMat4(const std::string& name, const glm::mat4& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);

private:
    unsigned int program_ = 0;
    std::unordered_map<std::string, int> uniformCache_;

    int getUniformLocation(const std::string& name);
    static unsigned int compileStage(unsigned int type,
                                      const std::string& source,
                                      const std::string& tag);
};

} // namespace rendering
