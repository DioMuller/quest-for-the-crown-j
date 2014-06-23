#pragma once
#include "PlayerInput.h"

namespace qfcbase
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
