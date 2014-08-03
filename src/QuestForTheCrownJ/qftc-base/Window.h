#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>

#include "Definitions.h"
#include "Interfaces.h"
#include "Shareable.h"

namespace qfcbase
{
	class Window :
		public Shareable<Window>
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			sf::Vector2i size;
			std::string title;
			sf::RenderWindow* window;
			std::shared_ptr<IRunnable> running;
			bool focused;
			static std::shared_ptr<Window> current;

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
			bool IsFocused();
			static std::shared_ptr<Window> GetCurrent();
			void SetTitle(std::string title);
	};
}