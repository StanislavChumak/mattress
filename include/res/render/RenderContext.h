#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "BufferObject.h"
#include "SpriteBatch.h"

#include <unordered_map>

namespace mtrs::res
{

class ShaderProgram;
class Texture;

class RenderContext
{
    BufferObject _quadVBO;
    BufferObject _quadEBO;

    std::unordered_map<uint64_t, SpriteBatch> _batches;
    std::vector<uint64_t> _keys;
public:

    RenderContext();
    RenderContext(const RenderContext&) = delete;
    RenderContext &operator=(const RenderContext&) = delete;
    RenderContext(RenderContext &&other) noexcept;
    RenderContext &operator=(RenderContext &&other) noexcept;
    ~RenderContext() noexcept = default;

    void create_sprite_batch(std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Texture> texture);
    void begin_batches();
    void submit_batch(uint64_t id, InstanceData date);
    void end_batches();
};

}

#endif
