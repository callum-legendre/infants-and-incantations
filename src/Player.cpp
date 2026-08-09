#include "Player.h"

void Player::OnUpdate(float deltaTime)
{
    InputState input = getInput();
}

InputState Player::getInput()
{
    // get the input state
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    InputState inputState {
        .forward = static_cast<bool>(keys[SDL_SCANCODE_W]),
        .left = static_cast<bool>(keys[SDL_SCANCODE_A]),
        .back = static_cast<bool>(keys[SDL_SCANCODE_S]),
        .right = static_cast<bool>(keys[SDL_SCANCODE_D])
    };

    return inputState;
}
