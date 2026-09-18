#include "res/render/SpriteBatch.hpp"

#include "glad/glad.h"

#include <cstring>

#include "util/fun/msg/mtrs_message.hpp"

namespace mtrs::res
{

SpriteBatch::SpriteBatch(const BufferObject &quad_VBO, const BufferObject &quad_EBO,
    uint32_t shader, uint32_t texture, uint64_t max_instances)
: _shader(shader), _texture(texture), _max_instances(max_instances)
{
    for(uint8_t i = 0; i < SpriteBatch::BUFFER_COUNT; i++)
    {
        _instance_vbo[i]._mode = GL_ARRAY_BUFFER;
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo[i]._id);
        glBufferStorage(GL_ARRAY_BUFFER, _max_instances * sizeof(InstanceData), nullptr,
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

        _mapped_buffers[i] = reinterpret_cast<InstanceData*>(glMapBufferRange(
            GL_ARRAY_BUFFER, 0, _max_instances * sizeof(InstanceData),
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
    }

    _vao.set_vertex_buffer(0, quad_VBO, sizeof(float) * 4, 0);
    _vao.add_attrib_float(0, 0, 2, 0);
    _vao.add_attrib_float(1, 0, 2, sizeof(float)*2);

    _vao.set_vertex_buffer(1, _instance_vbo[0], sizeof(InstanceData), 0);
    _vao.add_attrib_float (2, 1, 2, offsetof(InstanceData, position));
    _vao.add_attrib_float (3, 1, 2, offsetof(InstanceData, size));
    _vao.add_attrib_float (4, 1, 1, offsetof(InstanceData, rotation));
    _vao.add_attrib_float (5, 1, 2, offsetof(InstanceData, lb_uv));
    _vao.add_attrib_float (6, 1, 2, offsetof(InstanceData, rt_uv));
    _vao.add_attrib_byteN (7, 1, 4, offsetof(InstanceData, color));
    glVertexArrayBindingDivisor(_vao.id(), 1, 1);

    glVertexArrayElementBuffer(_vao.id(), quad_EBO._id);
}

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
{
    _vao = std::move(other._vao);
    instances = std::move(other.instances);
    layers = std::move(other.layers);
    for(uint8_t i = 0; i < BUFFER_COUNT; i++)
    {
        _instance_vbo[i] = std::move(other._instance_vbo[i]);
        _mapped_buffers[i] = other._mapped_buffers[i];
        _fences[i] = other._fences[i];
        other._mapped_buffers[i] = nullptr;
        other._fences[i] = 0;
    }
    _current_buffer_index = other._current_buffer_index;
    _shader = other._shader;
    _texture = other._texture;
    _max_instances = other._max_instances;
}

SpriteBatch &SpriteBatch::operator=(SpriteBatch &&other) noexcept
{
    if(this != &other)
    {
        _vao = std::move(other._vao);
        instances = std::move(other.instances);
        layers = std::move(other.layers);
        for(uint8_t i = 0; i < BUFFER_COUNT; i++)
        {
            _instance_vbo[i] = std::move(other._instance_vbo[i]);
            _mapped_buffers[i] = other._mapped_buffers[i];
            _fences[i] = other._fences[i];
            other._mapped_buffers[i] = nullptr;
            other._fences[i] = 0;
        }
        _current_buffer_index = other._current_buffer_index;
        _shader = other._shader;
        _texture = other._texture;
        _max_instances = other._max_instances;
    }
    return *this;
}

SpriteBatch::~SpriteBatch()
{
    instances.clear();
}


void SpriteBatch::begin_batch()
{
    if(instances.empty()) return;

    _current_buffer_index++;
    _current_buffer_index %= BUFFER_COUNT;

#ifndef FLAG_RELEASE
    if(instances.size() > _max_instances)
    {
        msg::mtrs_warning("The number of instances is ", instances.size(),
            ", which has exceeded the MAX_INSTANCES limit of ", _max_instances,
            " in the SpriteBatch resource.");
    }
#endif

    if(_fences[_current_buffer_index])
    {
        GLenum result = glClientWaitSync(_fences[_current_buffer_index], GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        if (result != GL_ALREADY_SIGNALED && result != GL_CONDITION_SATISFIED)
        {
            glWaitSync(_fences[_current_buffer_index], 0, GL_TIMEOUT_IGNORED);
        }
        glDeleteSync(_fences[_current_buffer_index]);
        _fences[_current_buffer_index] = 0;
    }
}

void SpriteBatch::draw_layer(float layer)
{
    if(instances.empty()) return;
    auto iter = instances.find(layer);
    if(iter == instances.end()) return;

    std::memcpy(_mapped_buffers[_current_buffer_index], iter->second.data(),
        iter->second.size() * sizeof(InstanceData));
    
    _vao.bind();
    _vao.set_vertex_buffer(1, _instance_vbo[_current_buffer_index], sizeof(InstanceData), 0);
    
    glUseProgram(_shader);
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)iter->second.size());
}

void SpriteBatch::end_batch()
{
    _fences[_current_buffer_index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    size_t count;
    for(auto &layer : layers)
    {
        auto &inst = instances.find(layer)->second;
        count = inst.size();
        inst.clear();
        if(count)
        {
            inst.reserve(count);
        }
    }
}

}
