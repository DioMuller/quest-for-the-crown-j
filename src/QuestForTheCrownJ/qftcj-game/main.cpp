#include <SFML/Graphics.hpp>

#ifdef _DEBUG
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-window-d.lib")
	#pragma comment(lib, "sfml-system-d.lib")
#else
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-window.lib")
	#pragma comment(lib, "sfml-system.lib")
#endif

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Quest for the Crown J");
	sf::Texture texture;
	sf::Sprite sprite;

	if (!texture.loadFromFile("Content/sprites/characters/bat.png")) return 1;
	
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}