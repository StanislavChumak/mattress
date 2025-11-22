#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "SpriteBatch.h"

#include <unordered_map>

class RenderContext
{
public:
    BufferObject quadVBO;
    BufferObject quadEBO;
    std::unordered_map<u_int64_t, SpriteBatch> batches;

    RenderContext();
    RenderContext(const RenderContext&) = delete;
    RenderContext &operator=(const RenderContext&) = delete;
    RenderContext(RenderContext &&other) noexcept;
    RenderContext &operator=(RenderContext &&other) noexcept;
    ~RenderContext() noexcept = default;
};

#endif
