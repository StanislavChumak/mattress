#include "comp/single/GlyphDecoder.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/Font.hpp"

#ifndef FLAG_RELEASE
#include "util/fun/msg/mtrs_message.hpp"
#endif

namespace mtrs::comp
{

GlyphDecoder::GlyphDecoder(res::ResourceManager &resource)
: _resources(resource)
{
    _fonts.push_back("null/");
    res::Glyph g = {' ', {{},{}}, {}};
    _glyph_map.emplace(g.symbol, std::pair{0, g});
    g.symbol = '\n';
    _glyph_map.emplace(g.symbol, std::pair{0, g});
    g.symbol = '\t';
    _glyph_map.emplace(g.symbol, std::pair{0, g});
}

void GlyphDecoder::submit_font(std::string path)
{
    for(auto &font : _fonts)
    {
        if(font == path) return;
    }
    auto font = _resources.get_resource<res::Font>(0, path);
    for(auto glyph : *font)
    {
        _glyph_map.emplace(glyph.symbol , std::pair{_fonts.size(), glyph});
    }
    _fonts.push_back(path);
}

std::pair<const std::string&, const res::Glyph&> GlyphDecoder::glyph(char32_t symbol)
{
    auto iter = _glyph_map.find(symbol);
#ifndef FLAG_RELEASE
    if(iter == _glyph_map.end())
    {
        msg::mtrs_error("GlyphDecoder doesn't have a single Font",
            "that represents a character with Unicode ", (uint32_t)symbol);
        return {"null/", {' ', {{},{}}, {}}};
    }
#endif
    return std::pair<const std::string&, const res::Glyph&>
        {_fonts[iter->second.first], iter->second.second};
}

res::Text GlyphDecoder::decode_text(std::u32string string)
{
    res::Text text;
    if(string.empty()) return text;
    text.string = std::move(string);

    auto g = glyph(text.string[0]);
    text.fonts.push_back(_resources.get_resource<res::Font>(0, g.first));
    text.glyphs.push_back({0, {}});
    text.space_size = g.second.size;
    const std::string *last_font = &g.first;
    for(char32_t c : text.string)
    {
        auto g = glyph(c);
        if(g.first == *last_font)
        {
            text.glyphs.rbegin()->second.push_back(std::move(g.second));
        }
        else
        {
            last_font = &g.first;
            size_t index = text.fonts.size();
            std::shared_ptr<res::Font> font = _resources.get_resource<res::Font>(0, g.first);
            for(size_t i = 0; i < text.fonts.size(); i++)
            {
                if(text.fonts[i] == font)
                {
                    index = i;
                    break;
                }
            }

            text.glyphs.push_back({index, {g.second}});
            if(index == text.fonts.size())
            {
                text.fonts.push_back(std::move(font));
            }
        }

        if(g.second.size.x > text.space_size.x) text.space_size.x = g.second.size.x;
        if(g.second.size.y > text.space_size.y) text.space_size.y = g.second.size.y;
    }

    return text;
}

}