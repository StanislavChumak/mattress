#include "resources/BufferObject.h"

#include "glad/glad.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

BufferObject::~BufferObject() noexcept
{
    if(_id) glDeleteBuffers(1, &_id);
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

void BufferObject::init(const unsigned int &mode, const void *data, unsigned int size, unsigned int usage)
{
    _mode = mode;
    glGenBuffers(1, &_id);
    glBindBuffer(_mode, _id);
    glBufferData(_mode, size, data, usage);
}

void BufferObject::update(const void *data, const unsigned int size, const unsigned int offset) const
{
#ifndef FLAG_RELEASE
    if (_id == 0) {
        std::cerr << "ERROR: Attempt to update uninitialized buffer." << std::endl;
        return;
    }
#endif
    glBindBuffer(_mode, _id);
    glBufferSubData(_mode, offset, size, data);
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

void BufferObject::bindBase(unsigned int index)
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