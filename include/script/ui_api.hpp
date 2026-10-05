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

#endif
