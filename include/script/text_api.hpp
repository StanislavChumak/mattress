#ifndef TEXT_API_HPP
#define TEXT_API_HPP

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

res::Text GlyphDecoder::decode_text(std::u32string string)
{
    return api->decoder_decode_text(this, string.c_str());
}

}

#endif
