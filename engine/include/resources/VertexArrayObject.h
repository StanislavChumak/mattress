#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include "glad/glad.h"


class BufferObject;

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    VertexArrayObject(const VertexArrayObject &) = delete;
    VertexArrayObject &operator=(const VertexArrayObject &) = delete;
    VertexArrayObject &operator=(VertexArrayObject &&vertexArrayObjectr) noexcept;
    VertexArrayObject(VertexArrayObject &&vertexArrayObjectr) noexcept;

    void add_buffer_float(const BufferObject &buffer, const unsigned int size, const unsigned int stride, const unsigned int offset);
    void bind() const;
    void unbind() const;

private:
    GLuint _id = 0;
    GLuint _index = 0;
};


#endif
