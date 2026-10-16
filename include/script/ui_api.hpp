#ifndef UI_API_HPP
#define UI_API_HPP

#include "engine_api.hpp"
#include "comp/single/GlyphDecoder.hpp"
#include "comp/ui/Label.hpp"
#include "res/text/Text.hpp"

extern mtrs::EngineAPI *api;

namespace mtrs::comp
{

void GlyphDecoder::submit_font(std::string path)
{
    api->decoder_submit_font(this, path.c_str());
}

bool GlyphDecoder::set_decode_text(res::Text &target, std::u32string source)
{
    return api->decoder_set_decode_text(this, &target, source.c_str());
}

}

namespace mtrs::str
{

inline std::u32string utf8_to_utf32(std::string_view text)
{
    return api->utf8_to_utf32(static_cast<std::string>(text).c_str());
}

}

#endif
