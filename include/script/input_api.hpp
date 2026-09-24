#ifndef INPUT_API_HPP
#define INPUT_API_HPP

#include "engine_api.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"
#include "comp/single/Cursor.hpp"

extern mtrs::EngineAPI *api;

namespace mtrs::comp
{

void KeyButtons::subscribe(int key, bool action, void(*callback)())
{
    api->key_subscribe(this, key, action, callback);
}

void KeyButtons::unsubscribe(int key, bool action, void(*callback)())
{
    api->key_unsubscribe(this, key, action, callback);
}

void MouseButtons::subscribe(int button, bool action, void(*callback)())
{
    api->mouse_subscribe(this, button, action, callback);
}

void MouseButtons::unsubscribe(int button, bool action, void(*callback)())
{
    api->mouse_unsubscribe(this, button, action, callback);
}

void MouseScroll::subscribe(void(*callback)())
{
    api->scroll_subscribe(this, callback);
}

void MouseScroll::unsubscribe(void(*callback)())
{
    api->scroll_unsubscribe(this, callback);
}

void Cursor::subscribe(void(*callback)())
{
    api->cursor_subscribe(this, callback);
}

void Cursor::unsubscribe(void(*callback)())
{
    api->cursor_unsubscribe(this, callback);
}

}

#endif