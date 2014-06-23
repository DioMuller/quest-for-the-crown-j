#include "PlayerInput.h"


qfcgame::PlayerInput::PlayerInput()
    : old_state(), current_state()
{
}


qfcgame::PlayerInput::~PlayerInput()
{
}

void qfcgame::PlayerInput::Update(double dt)
{
    old_state = current_state;
    current_state = GetState();
}