#include "resources/VertexArrayObject.h"

#include "resources/BufferObject.h"


VertexArrayObject::VertexArrayObject()
{
    glCreateVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject() noexcept
{
    if (_id != 0) glDeleteVertexArrays(1, &_id);
    _id = 0;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject &&other) noexcept
{
    _id = other._id;
    other._id = 0;
}

VertexArrayObject &VertexArrayObject::operator=(VertexArrayObject &&other) noexcept
{
    if(this != &other)
    {
        if (_id != 0) glDeleteVertexArrays(1, &_id);

        _id = other._id;
        other._id = 0;
    }
    return *this;
}

void VertexArrayObject::addBufferFloat(GLuint index, const BufferObject &buffer,
                                             GLint size, GLsizei stride, GLintptr offset)
{
    glVertexArrayVertexBuffer(_id, index, buffer.id(), offset, stride);

    glEnableVertexArrayAttrib(_id, index);
    glVertexArrayAttribBinding(_id, index, index);
    glVertexArrayAttribFormat(_id, index, size, GL_FLOAT, GL_FALSE, 0);
}

void VertexArrayObject::addBufferByteN(GLuint index, const BufferObject &buffer,
                                             GLint size, GLsizei stride, GLintptr offset)
{
    glVertexArrayVertexBuffer(_id, index, buffer.id(), offset, stride);

    glEnableVertexArrayAttrib(_id, index);
    glVertexArrayAttribBinding(_id, index, index);
    glVertexArrayAttribFormat(_id, index, size, GL_UNSIGNED_BYTE, GL_TRUE, 0);
}

void VertexArrayObject::bind() const
{
    glBindVertexArray(_id);
}

void VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
}