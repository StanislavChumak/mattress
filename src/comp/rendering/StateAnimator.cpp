#include "comp/rendering/StateAnimator.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_message.hpp"
#include "util/type/prs/comp/StateAnimator.hpp"

#include <cstring>
#include <vector>

namespace mtrs::comp
{

StateAnimator::StateAnimator(COMPONENT_ARGS)
{
    prs::StateAnimator state_anim;
    std::memcpy(&state_anim, file_data, sizeof(state_anim));

    size_t count = (reinterpret_cast<uint32_t*>(&state_anim.states))[1]/sizeof(prs::StateAnimator::State);
    states.reserve(count);

    current_state = 0;

    std::vector<prs::StateAnimator::State> buffer;
    prs::set_mtrs_to_var(file_ddata[state_anim.states], buffer);

    for(auto &state : buffer)
    {
        uint32_t id = state.id;
        states.emplace(id, State{state.offset, state.count});
        if(current_state == 0) current_state = id; 
    }
}

void StateAnimator::set_state(std::string state)
{
#ifndef FLAG_RELEASE
    auto iter = states.find(math::hash32(state));
    if(iter == states.end())
    {
        msg::mtrs_error("Animator does not have a state named: ", state);
        return;
    }
    current_state = iter->first;
#else
    current_state = math::hash32(state);
#endif
    dirty = true;
}

uint32_t StateAnimator::get_prs_size_imp() noexcept
{
    return sizeof(prs::StateAnimator);
}

}