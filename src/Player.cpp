#include "Player.h"

void Player::OnUpdate(float deltaTime)
{
    InputState input = getInput();

    
}

InputState Player::getInput()
{
    // get the input state
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    InputState* inputState {
        .forward = keys[SDL_SCANCODE_W],
        .left = keys[SLD_SCANCODE_A],
        .back = keys[SDL_SCANCODE_S],
        .right = keys[SDL_SCANCODE_D]
    };
}
