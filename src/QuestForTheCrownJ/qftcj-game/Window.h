#pragma once
#include "Definitions.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>


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
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Window(sf::Vector2i size, std::string title);
			~Window();
	};
}