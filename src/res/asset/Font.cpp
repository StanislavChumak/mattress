#include "res/asset/Font.hpp"

#include "res/ResourceManager.hpp"
#include "res/render/SubTexture.hpp"
#include "res/asset/Texture.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/msg/mtrs_message.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/str/utf8_to_utf32.hpp"
#include "util/type/prs/res/Font.hpp"

#include <string>
#include <cstring>

namespace mtrs::res
{

Font::Font(RESOURCE_ARGS)
{
    prs::Font font;
    std::memcpy(&font, file_data, sizeof(font));

    std::string str_buffer;

    prs::set_mtrs_to_var(file_ddata[font.texture], str_buffer);
    std::string pack_name = dir_pack.substr(resources.get_pack_dir().size()) + pack;
    texture = resources.get_resource<Texture>(math::hash64(pack_name), std::move(str_buffer));

    prs::set_mtrs_to_var(file_ddata[font.symbols], str_buffer);
    _symbols = str::utf8_to_utf32(std::move(str_buffer));

    prs::set_mtrs_to_var(file_ddata[font.symbol_widths], _symbol_widths);

    _symbol_height = font.symbol_height;
}

Font::Font(Font &&other) noexcept
{
    texture = std::move(other.texture);
    _symbols = std::move(other._symbols);
    _symbol_widths = std::move(other._symbol_widths);
    _symbol_height = other._symbol_height;
}

Font &Font::operator=(Font &&other) noexcept
{
    if(this != &other)
    {
        texture = std::move(other.texture);
        _symbols = std::move(other._symbols);
        _symbol_widths = std::move(other._symbol_widths);
        _symbol_height = other._symbol_height;
    }
    return *this;
}

uint32_t Font::get_prs_size_imp() noexcept
{
    return sizeof(prs::Font);
}

Font::Iterator::Iterator(const std::shared_ptr<Texture> &texture, const std::u32string &symbols,
    const std::vector<glm::uvec2> &symbol_widths, const uint32_t &symbol_height)
: _texture(texture), _symbols(symbols), _symbol_widths(symbol_widths), _symbol_height(symbol_height)
{
    _current_offset = glm::uvec2(0, texture->height());
    _current_width = symbol_widths[0].x;
}

bool Font::Iterator::operator!=(const Iterator& other) const noexcept
{
    return _index != other._index;
}

Font::Iterator& Font::Iterator::operator++()
{
    _index++;
    _current_offset.x += _current_width;
    if (_current_offset.x >= _texture->width())
    {
        _current_offset.x = 0;
        _current_offset.y -= _symbol_height;
    }
    _current_width = _index;
    for(auto &iter : _symbol_widths)
    {
        if(_current_width < iter.y)
        {
            _current_width = iter.x;
            break;
        } 
        else
        { 
            _current_width -= iter.y;
        }
    }
    
    return *this;
}

Glyph Font::Iterator::operator*() const
{
    glm::vec2 size = {_texture->width(), _texture->height()};
    Glyph glyph = {
        _symbols[_index],
        {
            glm::vec2(static_cast<float>(_current_offset.x + 0.01f) / size.x,
                static_cast<float>(_current_offset.y - _symbol_height + 0.01f) / size.y),
            glm::vec2(static_cast<float>(_current_offset.x + _current_width - 0.01f) / size.x, 
                static_cast<float>(_current_offset.y - 0.01f) / size.y)
        },
        glm::vec2{_current_width, _symbol_height}
    };
    return glyph;
}

Font::Iterator Font::begin() const
{
    return Font::Iterator(texture, _symbols, _symbol_widths, _symbol_height);
}

Font::Iterator Font::end() const
{
    Font::Iterator iter(texture, _symbols, _symbol_widths, _symbol_height);
    for(auto &symbol_width : _symbol_widths)
    {
        iter._index += symbol_width.y;
    }
    return iter;
}

}