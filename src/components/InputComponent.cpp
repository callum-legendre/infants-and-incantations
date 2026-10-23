#include "components/InputComponent.h"

#include <SDL2/SDL_keyboard.h>

void InputComponent::OnInput()
{
    // get the input state
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    mInput = {
        static_cast<bool>(keys[SDL_SCANCODE_W]),
        static_cast<bool>(keys[SDL_SCANCODE_A]),
        static_cast<bool>(keys[SDL_SCANCODE_S]),
        static_cast<bool>(keys[SDL_SCANCODE_D])
    };
}

InputState InputComponent::GetInput()
{
    return mInput;
}
