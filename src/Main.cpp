#include "Engine.hpp"

#include "util/fun/msg/mtrs_message.hpp"

#include <string>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::string argv0 = (argc > 0 && argv[0]) ? argv[0] : "";
    std::string executable_path = argv0.substr(0, argv0.find_last_of("/\\") + 1);

    std::replace(executable_path.begin(), executable_path.end(), '\\', '/');

#ifndef FLAG_RELEASE
    mtrs::msg::flag_message(mtrs::msg::FlagMessage::PRINT_INFO);
#endif

    mtrs::engine::Core engine(executable_path);

    engine.pre_update();
    while (!engine.is_close_window())
    {
        engine.update();
    }

    engine.shutdown();

    return 0;
}
