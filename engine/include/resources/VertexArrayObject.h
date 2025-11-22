#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include "glad/glad.h"

class BufferObject;

typedef const unsigned int cuint;

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    VertexArrayObject(const VertexArrayObject &) = delete;
    VertexArrayObject &operator=(const VertexArrayObject &) = delete;
    VertexArrayObject(VertexArrayObject &&other) noexcept;
    VertexArrayObject &operator=(VertexArrayObject &&other) noexcept;

    void addBufferFloat(cuint index, const BufferObject &buffer, cuint size, cuint stride, cuint offset);
    void addBufferByte(cuint index, const BufferObject &buffer, cuint size, cuint stride, cuint offset);
    void bind() const;
    void unbind() const;

private:
    GLuint _id = 0;
};


#endif
