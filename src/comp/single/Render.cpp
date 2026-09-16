#include "comp/single/Render.hpp"

#include "glad/glad.h"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include <algorithm>

namespace mtrs::comp
{

Render::Render()
{
    const float quad[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };
    _quad_VBO.init(GL_ARRAY_BUFFER, quad, sizeof(quad), GL_STATIC_DRAW);
    
    const GLuint indices[] = {0, 1, 2, 2, 3, 0};
    _quad_EBO.init(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_STATIC_DRAW);
}

void Render::submit_batch(const std::shared_ptr<res::ShaderProgram> &shader,
    const std::shared_ptr<res::Texture> &texture, float layer, std::vector<res::InstanceData> data)
{
    if(data.empty()) return;
    
    uint64_t id = shader->id() | (uint64_t(texture->id()) << 32);
    auto iter = _batches.find(id);

    if(iter == _batches.end())
    {
        res::SpriteBatch batch(_quad_VBO, _quad_EBO,
            shader->id(), texture->id(), texture->max_instances());
        batch.instances.emplace(layer, std::move(data));
        batch.layers.push_back(layer);
        _batches.emplace(id, std::move(batch));
        _batch_iters.push_back(_batches.find(id));
    }
    else
    {
        auto layer_iter = iter->second.instances.find(layer);
        if(layer_iter == iter->second.instances.end())
        {
            iter->second.instances.emplace(layer, std::move(data));
            auto pos = std::lower_bound(iter->second.layers.begin(),
                iter->second.layers.end(), layer,
                [] (const float &a, const float &b){ return a < b; });
            iter->second.layers.insert(pos, layer);
        }
        else
        {
            layer_iter->second.insert(layer_iter->second.end(),
                std::make_move_iterator(data.begin()), std::make_move_iterator(data.end()));
        }
    }
}

void Render::draw()
{
    _current_layers.reserve(_batch_iters.size());
    for(auto &iter : _batch_iters)
    {
        if(!iter->second.instances.empty())
        {
            iter->second.begin_batch();
            _current_layers.push_back({&iter->second, iter->second.layers.begin()});
        }
    }

    float layer;
    while(!_current_layers.empty())
    {
        layer = *_current_layers.begin()->second;
        for(auto iter = _current_layers.begin(); iter < _current_layers.end(); iter++)
        {
            if(*iter->second< layer)
            {
                layer = *iter->second;
            }
        }

        for(auto iter = _current_layers.begin(); iter < _current_layers.end();)
        {
            if(*iter->second == layer)
            {
                iter->first->draw_layer(*iter->second);
                iter->second++;
                if(iter->second == iter->first->layers.end())
                {
                    std::iter_swap(_current_layers.end() - 1, iter);
                    _current_layers.pop_back();
                }
                else iter++;
            }
            else iter++;
        }        
    }
    
    for(auto iter = _batch_iters.begin(); iter < _batch_iters.end();)
    {
        if((*iter)->second.instances.empty())
        {
            _batches.erase(*iter);
            std::iter_swap(_batch_iters.end() - 1, iter);
            _batch_iters.pop_back();
        }
        else
        {
            (*iter)->second.end_batch();
            iter++;
        }
    }
}

}