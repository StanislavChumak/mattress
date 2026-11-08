#include "res/render/BufferObject.hpp"

#include "glad/glad.h"

#include "util/fun/msg/mtrs_message.hpp"

namespace mtrs::res
{

BufferObject::BufferObject()
: _id(0), _mode(0)
{
    glGenBuffers(1, &_id);
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

void BufferObject::init(const uint32_t &mode, const void *data,
    const int64_t size, const uint32_t usage)
{
    _mode = mode;
    glBindBuffer(_mode, _id);
    glBufferData(_mode, size, data, usage);
}

void BufferObject::update(const void *data, const int64_t size, const int64_t offset) const
{
#ifndef FLAG_RELEASE
    if (_id == 0)
    {
        msg::mtrs_error("Attempt to update uninitialized buffer");
        return;
    }
#endif
    glBindBuffer(_mode, _id);
    glBufferSubData(_mode, offset, size, data);
}

void BufferObject::bind() const
{
#ifndef FLAG_RELEASE
    if (_id == 0)
    {
        msg::mtrs_error("Attempt to bind uninitialized buffer");
        return;
    }
#endif
    glBindBuffer(_mode, _id);
}

void BufferObject::bind_base(const uint32_t index)
{
#ifndef FLAG_RELEASE
    if (_id == 0)
    {
        msg::mtrs_error("Attempt to update uninitialized buffer");
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

}