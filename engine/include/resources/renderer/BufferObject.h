#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H


class BufferObject
{
public:
    BufferObject();
    ~BufferObject();

    BufferObject(const BufferObject &) = delete;
    BufferObject &operator=(const BufferObject &) = delete;
    BufferObject &operator=(BufferObject &&buffer) noexcept;
    BufferObject(BufferObject &&buffer) noexcept;

    void init(const unsigned int &mode, const void *data, unsigned int size, unsigned int usage);
    void update(const void *data, const unsigned int size, const unsigned int offset = 0) const;
    void bind() const;
    void unbind() const;
    int get_count() const { return _countVertex;};

private:
    unsigned int _id = 0;
    unsigned int _mode;
    int _countVertex;
};


#endif
