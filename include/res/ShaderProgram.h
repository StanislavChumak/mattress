#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glm/fwd.hpp>

#include "jsonUtils.h"

class ResourceManager;

class ShaderProgram
{
    bool createShader(const char *sourse, const unsigned int &shaderType, unsigned int &shaderID);
    bool _isCompiled = false;
    unsigned int _ID = 0;
public:
    void from_json(simdjson::ondemand::object obj, ResourceManager &resource);
    ShaderProgram() = default;
    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram(ShaderProgram &&other) noexcept;
    ShaderProgram &operator=(ShaderProgram &&other) noexcept;
    ~ShaderProgram();

    bool is_compiled() const { return _isCompiled; };
    void use() const;
    void set_int(const char* name, const int &value) const;
    void set_float(const char *name, const float &value) const;
    void set_matrix4(const char* name,  const glm::mat4 &matrix) const;
    bool has_uniform(const char *name) const;

    unsigned int id() const noexcept;    
};


#endif
