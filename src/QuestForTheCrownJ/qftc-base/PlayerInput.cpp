#include "PlayerInput.h"


qfcbase::PlayerInput::PlayerInput()
    : old_state(), current_state()
{
}


qfcbase::PlayerInput::~PlayerInput()
{
}

void qfcbase::PlayerInput::Update(double dt)
{
    old_state = current_state;
    current_state = GetState();
}