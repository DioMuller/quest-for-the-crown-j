#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include "Definitions.h"
#include "Game.h"

namespace qfcbase
{
	class Window
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			sf::Vector2i size;
			std::string title;
			sf::RenderWindow* window;
			Game* game;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Window(sf::Vector2i size, std::string title);
			~Window();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Run(Game* game);
	};
}