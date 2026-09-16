#ifndef KEY_BUTTONS_HPP
#define KEY_BUTTONS_HPP

#include "comp/Component.hpp"

#include <vector>
#include <unordered_map>

namespace mtrs::comp
{

struct KeyButtons : public Component<KeyButtons>
{
    bool keys[349];
    bool last_keys[349];
    std::unordered_map<uint32_t, std::vector<void(*)()>> subscribers;

    KeyButtons(const KeyButtons&) = delete;
    KeyButtons &operator=(const KeyButtons&) = delete;
    KeyButtons(KeyButtons&&) = delete;
    KeyButtons &operator=(KeyButtons&&) = delete;

    KeyButtons();
    ~KeyButtons() = default;

    void subscribe(int key, bool action, void(*callback)());
    void unsubscribe(int key, bool action, void(*callback)());

    static constexpr const char *get_type_name_imp() noexcept { return "KeyButtons"; }
};

}

#define MTRS_KEY_SPACE              32
#define MTRS_KEY_APOSTROPHE         39  /* ' */
#define MTRS_KEY_COMMA              44  /* , */
#define MTRS_KEY_MINUS              45  /* - */
#define MTRS_KEY_PERIOD             46  /* . */
#define MTRS_KEY_SLASH              47  /* / */
#define MTRS_KEY_0                  48
#define MTRS_KEY_1                  49
#define MTRS_KEY_2                  50
#define MTRS_KEY_3                  51
#define MTRS_KEY_4                  52
#define MTRS_KEY_5                  53
#define MTRS_KEY_6                  54
#define MTRS_KEY_7                  55
#define MTRS_KEY_8                  56
#define MTRS_KEY_9                  57
#define MTRS_KEY_SEMICOLON          59  /* ; */
#define MTRS_KEY_EQUAL              61  /* = */
#define MTRS_KEY_A                  65
#define MTRS_KEY_B                  66
#define MTRS_KEY_C                  67
#define MTRS_KEY_D                  68
#define MTRS_KEY_E                  69
#define MTRS_KEY_F                  70
#define MTRS_KEY_G                  71
#define MTRS_KEY_H                  72
#define MTRS_KEY_I                  73
#define MTRS_KEY_J                  74
#define MTRS_KEY_K                  75
#define MTRS_KEY_L                  76
#define MTRS_KEY_M                  77
#define MTRS_KEY_N                  78
#define MTRS_KEY_O                  79
#define MTRS_KEY_P                  80
#define MTRS_KEY_Q                  81
#define MTRS_KEY_R                  82
#define MTRS_KEY_S                  83
#define MTRS_KEY_T                  84
#define MTRS_KEY_U                  85
#define MTRS_KEY_V                  86
#define MTRS_KEY_W                  87
#define MTRS_KEY_X                  88
#define MTRS_KEY_Y                  89
#define MTRS_KEY_Z                  90
#define MTRS_KEY_LEFT_BRACKET       91  /* [ */
#define MTRS_KEY_BACKSLASH          92  /* \ */
#define MTRS_KEY_RIGHT_BRACKET      93  /* ] */
#define MTRS_KEY_GRAVE_ACCENT       96  /* ` */
#define MTRS_KEY_WORLD_1            161 /* non-US #1 */
#define MTRS_KEY_WORLD_2            162 /* non-US #2 */

#define MTRS_KEY_ESCAPE             256
#define MTRS_KEY_ENTER              257
#define MTRS_KEY_TAB                258
#define MTRS_KEY_BACKSPACE          259
#define MTRS_KEY_INSERT             260
#define MTRS_KEY_DELETE             261
#define MTRS_KEY_RIGHT              262
#define MTRS_KEY_LEFT               263
#define MTRS_KEY_DOWN               264
#define MTRS_KEY_UP                 265
#define MTRS_KEY_PAGE_UP            266
#define MTRS_KEY_PAGE_DOWN          267
#define MTRS_KEY_HOME               268
#define MTRS_KEY_END                269
#define MTRS_KEY_CAPS_LOCK          280
#define MTRS_KEY_SCROLL_LOCK        281
#define MTRS_KEY_NUM_LOCK           282
#define MTRS_KEY_PRINT_SCREEN       283
#define MTRS_KEY_PAUSE              284
#define MTRS_KEY_F1                 290
#define MTRS_KEY_F2                 291
#define MTRS_KEY_F3                 292
#define MTRS_KEY_F4                 293
#define MTRS_KEY_F5                 294
#define MTRS_KEY_F6                 295
#define MTRS_KEY_F7                 296
#define MTRS_KEY_F8                 297
#define MTRS_KEY_F9                 298
#define MTRS_KEY_F10                299
#define MTRS_KEY_F11                300
#define MTRS_KEY_F12                301
#define MTRS_KEY_F13                302
#define MTRS_KEY_F14                303
#define MTRS_KEY_F15                304
#define MTRS_KEY_F16                305
#define MTRS_KEY_F17                306
#define MTRS_KEY_F18                307
#define MTRS_KEY_F19                308
#define MTRS_KEY_F20                309
#define MTRS_KEY_F21                310
#define MTRS_KEY_F22                311
#define MTRS_KEY_F23                312
#define MTRS_KEY_F24                313
#define MTRS_KEY_F25                314
#define MTRS_KEY_KP_0               320
#define MTRS_KEY_KP_1               321
#define MTRS_KEY_KP_2               322
#define MTRS_KEY_KP_3               323
#define MTRS_KEY_KP_4               324
#define MTRS_KEY_KP_5               325
#define MTRS_KEY_KP_6               326
#define MTRS_KEY_KP_7               327
#define MTRS_KEY_KP_8               328
#define MTRS_KEY_KP_9               329
#define MTRS_KEY_KP_DECIMAL         330
#define MTRS_KEY_KP_DIVIDE          331
#define MTRS_KEY_KP_MULTIPLY        332
#define MTRS_KEY_KP_SUBTRACT        333
#define MTRS_KEY_KP_ADD             334
#define MTRS_KEY_KP_ENTER           335
#define MTRS_KEY_KP_EQUAL           336
#define MTRS_KEY_LEFT_SHIFT         340
#define MTRS_KEY_LEFT_CONTROL       341
#define MTRS_KEY_LEFT_ALT           342
#define MTRS_KEY_LEFT_SUPER         343
#define MTRS_KEY_RIGHT_SHIFT        344
#define MTRS_KEY_RIGHT_CONTROL      345
#define MTRS_KEY_RIGHT_ALT          346
#define MTRS_KEY_RIGHT_SUPER        347
#define MTRS_KEY_MENU               348

#endif