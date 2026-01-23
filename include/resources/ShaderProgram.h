#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glm/fwd.hpp>

#include "../external/jsonUtils.h"

class ResourceManager;

class ShaderProgram
{
public:
    void fromJson(simdjson::ondemand::object obj, ResourceManager &resource);
    ShaderProgram() = default;
    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram(ShaderProgram &&other) noexcept;
    ShaderProgram &operator=(ShaderProgram &&other) noexcept;
    ~ShaderProgram();

    bool isCompiled() const { return _isCompiled; };
    void use() const;
    void setInt(const char* name, const int &value) const;
    void setFloat(const char *name, const float &value) const;
    void setMatrix4(const char* name,  const glm::mat4 &matrix) const;
    bool hasUniform(const char *name) const;

    unsigned int id() const noexcept;
private:

    bool createShader(const char *sourse, const unsigned int &shaderType, unsigned int &shaderID);
    bool _isCompiled = false;
    unsigned int _ID = 0;
};


#endif
