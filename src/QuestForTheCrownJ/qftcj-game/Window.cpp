#include "Window.h"
#include "Definitions.h"

using namespace qfcbase;

Window::Window(sf::Vector2i size, std::string title)
{
	this->size = size;
	this->title = title;
	this->window = new sf::RenderWindow(sf::VideoMode(size.x, size.y), title);	
}


Window::~Window()
{
	delete window;
	delete game;
}

void Window::Run(Game* game)
{
	// Sets the game.
	this->game = game;

	//Initial update duration = 1 frame.
	double delta = MILISECONDS_PER_FRAME;

	while (window->isOpen())
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();

		sf::Event windowEvent;
		while (window->pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
				window->close();
		}

		window->clear();
		game->Update(delta);
		game->Draw(window);
		window->display();
		end = std::chrono::system_clock::now();

		delta = ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / (1000.0 * 1000.0);

		if (delta < MILISECONDS_PER_FRAME)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((long long)(MILISECONDS_PER_FRAME - delta) * (1000 * 1000)));
			delta = MILISECONDS_PER_FRAME;
		}
	}
}