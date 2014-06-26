#pragma once
#include <SFML\System\Vector2.hpp>

namespace qfcgame
{
	const float AnalogToDigital = 0.4f;

	typedef struct input_state
	{
	public:
		input_state() : movement({ 0, 0 }), action(false), run(false), open_menu(false)
		{

		}

		sf::Vector2f movement;
		bool action, run, open_menu;
	};

	class PlayerInput
	{
	private:
		input_state old_state, current_state;

	protected:
		virtual input_state GetState() = 0;

	public:
		PlayerInput();
		~PlayerInput();

		void Update(double dt);

		// GamePlay
		sf::Vector2f Movement() { return current_state.movement; }

		bool Run() { return current_state.run; }
		bool Action() { return current_state.action; }

		// Menu
		bool MenuUp() { return current_state.movement.y < -AnalogToDigital && !(old_state.movement.y < -AnalogToDigital); }
		bool MenuDown() { return current_state.movement.y > AnalogToDigital && !(old_state.movement.y > AnalogToDigital); }
		bool MenuLeft() { return current_state.movement.x < -AnalogToDigital && !(old_state.movement.x < -AnalogToDigital); }
		bool MenuRight() { return current_state.movement.x > AnalogToDigital && !(old_state.movement.x > AnalogToDigital); }

		bool MenuOpen() { return current_state.open_menu && !old_state.open_menu; }
		bool MenuAccept() { return current_state.action && !old_state.action; }
		bool MenuCancel() { return current_state.run && !old_state.run; }
	};
}