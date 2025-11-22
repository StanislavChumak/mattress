#include "resources/VertexArrayObject.h"

#include "resources/BufferObject.h"


VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject() noexcept
{
    glDeleteVertexArrays(1, &_id);
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
        _id = other._id;
        other._id = 0;
    }
    return *this;
}

void VertexArrayObject::addBufferFloat(cuint index, const BufferObject &buffer, cuint size, cuint stride, cuint offset)
{
    bind();
    buffer.bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, static_cast<GLint>(size), GL_FLOAT, GL_FALSE, stride, (GLvoid *)offset);
}

void VertexArrayObject::addBufferByte(cuint index, const BufferObject &buffer, cuint size, cuint stride, cuint offset)
{
    bind();
    buffer.bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, static_cast<GLint>(size), GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid *)offset);
}

void VertexArrayObject::bind() const
{
    glBindVertexArray(_id);
}

void VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
}
