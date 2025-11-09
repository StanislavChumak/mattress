#include "resources/VertexArrayObject.h"

#include "resources/BufferObject.h"


VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &_id);
}

VertexArrayObject &VertexArrayObject::operator=(VertexArrayObject &&vertexArrayObjectr) noexcept
{
    _id = vertexArrayObjectr._id;
    vertexArrayObjectr._id = 0;
    return *this;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject &&vertexArrayObjectr) noexcept
{
    _id = vertexArrayObjectr._id;
    vertexArrayObjectr._id = 0;
}

void VertexArrayObject::add_buffer_float(const BufferObject &buffer, const unsigned int size, const unsigned int stride, const unsigned int offset)
{
    bind();
    buffer.bind();
    glEnableVertexAttribArray(_index);
    glVertexAttribPointer(_index, static_cast<GLint>(size), GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(offset * sizeof(GLfloat)));
    _index++;
}

void VertexArrayObject::bind() const
{
    glBindVertexArray(_id);
}

void VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
}
