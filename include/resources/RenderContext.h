#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "BufferObject.h"
#include "SpriteBatch.h"

#include <unordered_map>

class ShaderProgram;
class Texture2D;
struct Transform;
struct Sprite2D;

class RenderContext
{
    BufferObject _quadVBO;
    BufferObject _quadEBO;

    std::unordered_map<u_int64_t, SpriteBatch> _batches;
    std::vector<u_int64_t> _keys;
public:

    RenderContext();
    RenderContext(const RenderContext&) = delete;
    RenderContext &operator=(const RenderContext&) = delete;
    RenderContext(RenderContext &&other) noexcept;
    RenderContext &operator=(RenderContext &&other) noexcept;
    ~RenderContext() noexcept = default;

    void createSpriteBatch(std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Texture2D> texture);
    void beginBatches();
    void submitBatch(u_int64_t id, Transform *transform, Sprite2D *sprite);
    void endBatches();
};

#endif
