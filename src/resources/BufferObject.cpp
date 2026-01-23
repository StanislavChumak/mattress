#include "resources/BufferObject.h"

#include "glad/glad.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

BufferObject::BufferObject()
{
    glCreateBuffers(1, &_id);
}

BufferObject::~BufferObject() noexcept
{
    if(_id) glDeleteBuffers(1, &_id);
    _id = 0;
}

BufferObject &BufferObject::operator=(BufferObject &&other) noexcept
{
    if(this != &other)
    {
        if(_id) glDeleteBuffers(1, &_id);
        _id = other._id;
        _mode = other._mode;
        other._id = 0;
        other._mode = 0;
    }
    return *this;
}

BufferObject::BufferObject(BufferObject &&other) noexcept
{
    _id = other._id;
    _mode = other._mode;
    other._id = 0;
    other._mode = 0;
}

void BufferObject::init(const GLenum &mode, const void *data, const GLsizeiptr size, const GLenum usage)
{
    _mode = mode;
    glNamedBufferData(_id, size, data, usage);
}

void BufferObject::update(const void *data, const GLsizeiptr size, const GLintptr offset) const
{
#ifndef FLAG_RELEASE
    if (_id == 0) {
        std::cerr << "ERROR: Attempt to update uninitialized buffer." << std::endl;
        return;
    }
#endif
    glNamedBufferSubData(_id, offset, size, data);
}

void BufferObject::bind() const
{
#ifndef FLAG_RELEASE
    if (_id == 0) {
        std::cerr << "ERROR: Attempt to bind uninitialized buffer." << std::endl;
        return;
    }
#endif
    glBindBuffer(_mode, _id);
}

void BufferObject::bindBase(const GLuint index)
{
#ifndef FLAG_RELEASE
    if (_id == 0) {
        std::cerr << "ERROR: Attempt to update uninitialized buffer." << std::endl;
        return;
    }
#endif
    glBindBuffer(_mode, _id);
    glBindBufferBase(_mode, index, _id);
}

void BufferObject::unbind() const noexcept
{
    glBindBuffer(_mode, 0);
}