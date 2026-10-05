#ifndef GLYPH_DECODER_HPP
#define GLYPH_DECODER_HPP

#include "comp/Component.hpp"

#include "res/text/Text.hpp"

#include <unordered_map>
#include <vector>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::comp
{

struct GlyphDecoder : public Component<GlyphDecoder>
{
private:
    res::ResourceManager &_resources;
    std::vector<std::string> _fonts;
    std::unordered_map<char32_t, std::pair<size_t, res::Glyph>> _glyph_map;

public:
    GlyphDecoder() = delete;
    GlyphDecoder(const GlyphDecoder&) = delete;
    GlyphDecoder &operator=(const GlyphDecoder&) = delete;
    GlyphDecoder(GlyphDecoder &&other) = delete;
    GlyphDecoder &operator=(GlyphDecoder &&other) = delete;
    
    GlyphDecoder(res::ResourceManager &resource);
    ~GlyphDecoder() noexcept = default;

    void submit_font(std::string path);
    std::pair<const std::string&, const res::Glyph&> glyph(char32_t symbol);
    bool set_decode_text(res::Text &target, std::u32string source);

    static constexpr const char *get_type_name_imp() noexcept { return "GlyphDecoder"; }
};

}

#endif
