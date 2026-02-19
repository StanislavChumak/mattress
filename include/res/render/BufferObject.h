#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "glad/glad.h"

class BufferObject
{
    friend class RenderContext;

    GLuint _id;
    GLenum _mode;
public:
    BufferObject();
    ~BufferObject() noexcept;

    BufferObject(const BufferObject &) = delete;
    BufferObject &operator=(const BufferObject &) = delete;
    BufferObject(BufferObject &&other) noexcept;
    BufferObject &operator=(BufferObject &&other) noexcept;

    GLuint id() const noexcept { return _id; }

    void init(const GLenum &mode, const void *data, const GLsizeiptr size, const GLenum usage);
    void update(const void *data, const GLsizeiptr size, const GLintptr offset) const;

    void bind() const;
    void bind_base(const GLuint index);
    void unbind() const noexcept;
};


#endif
