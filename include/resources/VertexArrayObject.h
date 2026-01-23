#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include "glad/glad.h"

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

    void addBufferFloat(GLuint index, const BufferObject &buffer, GLint size, GLsizei stride, GLintptr offset);
    void addBufferByteN(GLuint index, const BufferObject &buffer, GLint size, GLsizei stride, GLintptr offset);

    void bind() const;
    void unbind() const;
};


#endif
