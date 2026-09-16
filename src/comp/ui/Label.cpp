#include "comp/ui/Label.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ShaderProgram.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/GlyphDecoder.hpp"

#include "util/fun/str/utf8_to_utf32.hpp"
#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/math/hash.hpp"
#include "util/type/prs/comp/Label.hpp"

#include <cstring>

namespace mtrs::comp
{

Label::Label(COMPONENT_ARGS)
{
    prs::Label label;
    std::memcpy(&label, file_data, sizeof(label));
    
    std::string str;

    prs::set_mtrs_to_var(file_ddata[label.shader], str);
    shader = resource.get_resource<res::ShaderProgram>(tmp_hash, std::move(str));
    
    prs::set_mtrs_to_var(file_ddata[label.text], str);
    auto decoder = static_cast<GlyphDecoder*>(world.single_comp(math::hash64(GlyphDecoder::get_type_name())));
    text = decoder->decode_text(str::utf8_to_utf32(str));

    layer = label.layer;

    size.x = label.size_x;
    size.y = label.size_y;

    std::memcpy(&color, &label.color, 4);

    visibility = label.visibility;
}

uint32_t Label::get_prs_size_imp() noexcept
{
    return sizeof(prs::Label);
}

}