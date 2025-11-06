#include "resources/renderer/BufferObject.h"

#include "glad/glad.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif


BufferObject::BufferObject(){}
BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &_id);
}

BufferObject &BufferObject::operator=(BufferObject &&buffer) noexcept
{
    _id = buffer._id;
    _mode = buffer._mode;
    if(_mode == GL_ELEMENT_ARRAY_BUFFER)
    {
        _countVertex = buffer._countVertex;
        buffer._countVertex = 0;
    }
    buffer._id = 0;
    buffer._mode = 0;
    return *this;
}

BufferObject::BufferObject(BufferObject &&buffer) noexcept
{
    _id = buffer._id;
    _mode = buffer._mode;
    if(_mode == GL_ELEMENT_ARRAY_BUFFER)
    {
        _countVertex = buffer._countVertex;
        buffer._countVertex = 0;
    }
    buffer._id = 0;
    buffer._mode = 0;
}

void BufferObject::init(const unsigned int &mode, const void *data, unsigned int size, unsigned int usage)
{
    _mode = mode;
    glGenBuffers(1, &_id);
    glBindBuffer(_mode, _id);
    glBufferData(_mode, size, data, usage);
    if(_mode == GL_ELEMENT_ARRAY_BUFFER) _countVertex = size / sizeof(unsigned int);
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

void BufferObject::unbind() const
{
    glBindBuffer(_mode, 0);
}