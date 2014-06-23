#pragma once
#include "PlayerInput.h"

namespace qfcgame
{
    class KeyboardInput :
        public PlayerInput
    {
    protected:
        virtual input_state GetState();

    public:
        KeyboardInput();
        ~KeyboardInput();
    };
}
