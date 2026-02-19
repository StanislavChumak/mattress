#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include "glad/glad.h"

namespace mtrs::res
{

class BufferObject;

class VertexArrayObject
{
private:
    GLuint _id = 0;
public:
    VertexArrayObject();
    ~VertexArrayObject() noexcept;

    VertexArrayObject(const VertexArrayObject &) = delete;
    VertexArrayObject &operator=(const VertexArrayObject &) = delete;
    VertexArrayObject(VertexArrayObject &&other) noexcept;
    VertexArrayObject &operator=(VertexArrayObject &&other) noexcept;

    GLuint id() const noexcept { return _id; }

    void add_buffer_float(GLuint index, const BufferObject &buffer, GLint size, GLsizei stride, GLintptr offset);
    void add_buffer_byteN(GLuint index, const BufferObject &buffer, GLint size, GLsizei stride, GLintptr offset);

    void bind() const;
    void unbind() const;
};


}

#endif
