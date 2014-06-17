#include "Definitions.h"
#include "AnimatedSprite.h"
#include <chrono>
#include <thread>

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600), "Quest for the Crown J");
	qfcbase::AnimatedSprite sprite("Content/sprites/characters/bat.png", sf::Vector2i{ 18, 16 }, sf::Vector2f{ 100.0f, 100.0f });
	
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
		sprite.Update(delta);
		sprite.Render(window);
		window->display();
		end = std::chrono::system_clock::now();

		delta = ((double) std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / (1000.0 * 1000.0);

		if (delta < MILISECONDS_PER_FRAME)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((long long) (MILISECONDS_PER_FRAME - delta) * (1000 * 1000)));
			delta = MILISECONDS_PER_FRAME;
		}
	}

	delete window;

	return 0;
}