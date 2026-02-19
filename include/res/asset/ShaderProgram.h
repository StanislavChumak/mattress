#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glm/fwd.hpp>

#include "res/asset/Asset.h"

namespace mtrs::res
{

class ResourceManager;

class ShaderProgram : public Asset
{
    bool createShader(const char *sourse, const unsigned int &shaderType, unsigned int &shaderID);
    bool _isCompiled = false;
    unsigned int _ID = 0;
public:
    ASSETS_CONSTRUCTORS(ShaderProgram);

    std::string get_type_name() override;
    uint32_t get_type_size() override;

    bool is_compiled() const { return _isCompiled; };
    void use() const;
    void set_int(const char* name, const int &value) const;
    void set_float(const char *name, const float &value) const;
    void set_matrix4(const char* name,  const glm::mat4 &matrix) const;
    bool has_uniform(const char *name) const;

    unsigned int id() const noexcept;    
};

}

#endif
