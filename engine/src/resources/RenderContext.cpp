#include "resources/RenderContext.h"

#include "glad/glad.h"

RenderContext::RenderContext()
{
    const float quad[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };
    quadVBO.init(GL_ARRAY_BUFFER, quad, sizeof(quad), GL_STATIC_DRAW);
    
    const GLuint indices[] = {0, 1, 2, 2, 3, 0};
    quadEBO.init(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_STATIC_DRAW);
}

RenderContext::RenderContext(RenderContext &&other) noexcept
{
    quadEBO = std::move(other.quadEBO);
    quadVBO = std::move(other.quadVBO);
    batches = std::move(other.batches);
}

RenderContext &RenderContext::operator=(RenderContext &&other) noexcept
{
    if(this != &other)
    {
        quadEBO = std::move(other.quadEBO);
        quadVBO = std::move(other.quadVBO);
        batches = std::move(other.batches);
    }
    return *this;
}