#include "res/render/VertexArrayObject.hpp"

#include "res/render/BufferObject.hpp"

namespace mtrs::res
{

VertexArrayObject::VertexArrayObject()
: _id(0)
{
    glGenVertexArrays(1, &_id);
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

void VertexArrayObject::add_attrib_float(GLuint attrib_index,
    GLint size, GLsizei stride, GLintptr rel_offset)
{
    glEnableVertexAttribArray(attrib_index);
    glVertexAttribPointer(attrib_index, size, GL_FLOAT, GL_FALSE, stride,
        reinterpret_cast<const void *>(rel_offset));
}

void VertexArrayObject::add_attrib_byteN(GLuint attrib_index,
    GLint size, GLsizei stride, GLintptr rel_offset)
{
    glEnableVertexAttribArray(attrib_index);
    glVertexAttribPointer(attrib_index, size, GL_UNSIGNED_BYTE, GL_TRUE, stride,
        reinterpret_cast<const void *>(rel_offset));
}

void VertexArrayObject::set_divisor(GLuint attrib_index, GLuint divisor)
{
    glVertexAttribDivisor(attrib_index, divisor);
}

void VertexArrayObject::set_vertex_buffer(const BufferObject &buffer)
{
    buffer.bind();
}

void VertexArrayObject::set_index_buffer(const BufferObject &buffer)
{
    glBindVertexArray(_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id());
}

void VertexArrayObject::bind() const
{
    glBindVertexArray(_id);
}

void VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
}

}