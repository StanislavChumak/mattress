#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

class BufferObject
{
public:
    BufferObject() = default;
    ~BufferObject() noexcept;

    BufferObject(const BufferObject &) = delete;
    BufferObject &operator=(const BufferObject &) = delete;
    BufferObject(BufferObject &&other) noexcept;
    BufferObject &operator=(BufferObject &&other) noexcept;

    void init(const unsigned int &mode, const void *data, unsigned int size, unsigned int usage);
    void update(const void *data, const unsigned int size, const unsigned int offset = 0) const;
    void bind() const;
    void bindBase(unsigned int index);
    void unbind() const noexcept;

private:
    unsigned int _id = 0;
    unsigned int _mode;
};


#endif
