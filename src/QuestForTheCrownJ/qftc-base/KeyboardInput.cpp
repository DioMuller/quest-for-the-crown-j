#include "KeyboardInput.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>
#include <math.h>
#include "Window.h"

qfcgame::KeyboardInput::KeyboardInput()
{
}


qfcgame::KeyboardInput::~KeyboardInput()
{
}

qfcgame::input_state qfcgame::KeyboardInput::GetState()
{
    input_state state;
	if (qfcbase::Window::GetCurrent()->IsFocused())
	{
		state.movement = GetMovement();
		state.action = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
		state.run = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		state.open_menu = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
	}
    return state;
}

sf::Vector2f qfcgame::KeyboardInput::GetMovement()
{
	sf::Vector2f movement;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		movement.y -= 1;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		movement.y += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		movement.x -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		movement.x += 1;

	if (movement.x || movement.y)
		return movement / sqrtf(movement.x * movement.x + movement.y * movement.y);
	return movement;
}
