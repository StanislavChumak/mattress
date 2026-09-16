#ifndef RENDER_HPP
#define RENDER_HPP

#include "comp/Component.hpp"
#include "res/render/SpriteBatch.hpp"

#include <unordered_map>

namespace mtrs::res
{
    class ShaderProgram;
    class Texture;
}

namespace mtrs::comp
{

struct Render : public Component<Render>
{
private:
    res::BufferObject _quad_VBO;
    res::BufferObject _quad_EBO;

    std::unordered_map<uint64_t, res::SpriteBatch> _batches;
    std::vector<decltype(_batches)::iterator> _batch_iters;
    std::vector<std::pair<res::SpriteBatch*, decltype(res::SpriteBatch::layers)::const_iterator>> _current_layers;
    
public:
    Render(const Render&) = delete;
    Render &operator=(const Render&) = delete;
    Render(Render &&other) = delete;
    Render &operator=(Render &&other) = delete;

    Render();
    ~Render() = default;

    void submit_batch(const std::shared_ptr<res::ShaderProgram> &shader,
        const std::shared_ptr<res::Texture> &texture, float layer, std::vector<res::InstanceData> date);
    void draw();

    static constexpr const char *get_type_name_imp() noexcept { return "Render"; }
};

}

#endif
