#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>

#include "Definitions.h"
#include "Interfaces.h"

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
			std::shared_ptr<IRunnable> running;

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
			void Run(std::shared_ptr<IRunnable> runnable);
	};
}