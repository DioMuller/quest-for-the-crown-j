#pragma once
#include "PlayerInput.h"

namespace qfcgame
{
    class KeyboardInput :
        public PlayerInput
    {
    protected:
        virtual input_state GetState();
	private:
		sf::Vector2f GetMovement();

    public:
        KeyboardInput();
        ~KeyboardInput();
    };
}
