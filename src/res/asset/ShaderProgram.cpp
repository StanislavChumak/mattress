#include "res/ShaderProgram.h"

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>

#ifndef FLAG_RELEASE
#include <iostream>
#endif

std::string getFileString(const std::string &relativeFilePath)
{
    std::ifstream fileFlow;
    fileFlow.open(relativeFilePath, std::ios::ate | std::ios::binary);

#   ifndef FLAG_RELEASE
    if (!fileFlow.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return "";
    }
#   endif

    std::streamsize size = fileFlow.tellg();
    fileFlow.seekg(0);

    char* buffer = new char[size + 1];
    fileFlow.read(buffer, size);
    buffer[size] = '\0';

    return std::string(buffer);
}

void ShaderProgram::from_json(simdjson::ondemand::object obj, ResourceManager &resource)
{
    std::string vertexShaderCode = getFileString(std::string(obj["pathVertexFile"].get_string().value()));
    std::string fragmentShaderCode = getFileString(std::string(obj["pathFragmentFile"].get_string().value()));

#   ifndef FLAG_RELEASE
    if (vertexShaderCode.empty())
    {
        std::cerr << "No vertex shader !" << std::endl;
        return;
    }
    
    if (fragmentShaderCode.empty())
    {
        std::cerr << "No fragment shader !" << std::endl;
        return;
    }
#   endif

    GLuint vertexShaderID = 0;
    GLuint fragmentShaderID = 0;
    if (!createShader(std::move(vertexShaderCode).c_str(), GL_VERTEX_SHADER, vertexShaderID))
    {
#ifndef FLAG_RELEASE
        std::cerr << "ERROR::VERTEX::SHADER::COMPILATION_FAILED" << std::endl;
        return;
#endif
    }

    if (!createShader(std::move(fragmentShaderCode).c_str(), GL_FRAGMENT_SHADER, fragmentShaderID))
    {
#ifndef FLAG_RELEASE
        std::cerr << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED" << std::endl;
        glDeleteShader(vertexShaderID);
        return;
#endif
    }

    _ID = glCreateProgram();
    glAttachShader(_ID, vertexShaderID);
    glAttachShader(_ID, fragmentShaderID);
    glLinkProgram(_ID);
#ifndef FLAG_RELEASE
    GLint success = 0;
    glGetProgramiv(_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(_ID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED: \n"
                    << infoLog << std::endl;
        _isCompiled = false;
    }
    else
#endif
    {
        _isCompiled = true;
    }
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

bool ShaderProgram::createShader(const char *sourse, const unsigned int &shaderType, unsigned int &shaderID)
{
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &sourse, nullptr);
    glCompileShader(shaderID);
#ifndef FLAG_RELEASE
    GLint success = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED (shader type " << shaderType << "):\n"
                    << infoLog << std::endl;
        return false;
    }
#endif
    return true;
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept
{
    _ID = other._ID;
    _isCompiled = other._isCompiled;
    other._ID = 0;
    other._isCompiled = false;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept
{
    if(this != &other)
    {
        glDeleteProgram(_ID);
        _ID = other._ID;
        _isCompiled = other._isCompiled;
        other._ID = 0;
        other._isCompiled = false;
    }
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_ID);
}

void ShaderProgram::use() const
{
    glUseProgram(_ID);
}

void ShaderProgram::set_int(const char *name, const int &value) const
{
    glUniform1i(glGetUniformLocation(_ID, name), value);
}

void ShaderProgram::set_float(const char *name, const float &value) const
{
    glUniform1f(glGetUniformLocation(_ID, name), value);
}

void ShaderProgram::set_matrix4(const char *name, const glm::mat4 &matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(_ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

bool ShaderProgram::has_uniform(const char *name) const {
    return glGetUniformLocation(_ID, name) != -1;
}

unsigned int ShaderProgram::id() const noexcept
{
    return _ID;
}