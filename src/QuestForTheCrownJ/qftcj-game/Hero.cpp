#include "Hero.h"
#include "AnimatedSprite.h"
#include "Container.h"
#include <math.h>

qfcgame::Hero::Hero()
{
	Speed = 5 * 32;
	const double walkFrameDuration = 0.1;

	auto sprite = std::make_shared<qfcbase::AnimatedSprite>("Content/sprites/characters/main.png", sf::Vector2i(64, 64));
	sprite->AddAnimation("stopped_up", { { 0, 0 }, { 0, 0 }, walkFrameDuration });
	sprite->AddAnimation("stopped_down", { { 0, 2 }, { 0, 2 }, walkFrameDuration });
	sprite->AddAnimation("stopped_left", { { 0, 1 }, { 0, 1 }, walkFrameDuration });
	sprite->AddAnimation("stopped_right", { { 0, 3 }, { 0, 3 }, walkFrameDuration });

	sprite->AddAnimation("walking_up", { { 1, 0 }, { 8, 0 }, walkFrameDuration });
	sprite->AddAnimation("walking_down", { { 1, 2 }, { 8, 2 }, walkFrameDuration });
	sprite->AddAnimation("walking_left", { { 1, 1 }, { 8, 1 }, walkFrameDuration });
	sprite->AddAnimation("walking_right", { { 1, 3 }, { 8, 3 }, walkFrameDuration });

	sprite->AddAnimation("dying", { { 0, 12 }, { 5, 12 }, walkFrameDuration });
	sprite->CurrentAnimation = "stopped_down";
	Sprite = sprite;

	sprite->Padding = sf::FloatRect(22, 32, 22, 2);
}


qfcgame::Hero::~Hero()
{
}


void qfcgame::Hero::Draw(sf::RenderWindow* renderer)
{
	sf::Vector2f cameraPosition = Sprite->Position;
	sf::Vector2f screenSize = sf::Vector2f(renderer->getSize().x, renderer->getSize().y);
	
	//TODO: Change fixed map size?
	// Not on the same line because... It won't work. Not sure why.
	cameraPosition.x = max(cameraPosition.x, screenSize.x / 2);
	cameraPosition.x = min(cameraPosition.x, 1920 - screenSize.x /2);
	cameraPosition.y = max(cameraPosition.y, screenSize.y / 2);
	cameraPosition.y = min(cameraPosition.y, 1080 - screenSize.y / 2);

	renderer->setView(sf::View(cameraPosition, screenSize ));
	Entity::Draw(renderer);
}