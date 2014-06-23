#include "KeyboardInput.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>

qfcbase::KeyboardInput::KeyboardInput()
{
}


qfcbase::KeyboardInput::~KeyboardInput()
{
}

qfcbase::input_state qfcbase::KeyboardInput::GetState()
{
    input_state state;
    state.move_up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    state.move_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    state.move_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    state.move_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    state.action = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    state.run = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    state.open_menu = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
    return state;
}