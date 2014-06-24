#include "Hero.h"
#include "AnimatedSprite.h"
#include "Container.h"

qfcgame::Hero::Hero()
{
	const double walkFrameDuration = 100;

	auto sprite = new qfcbase::AnimatedSprite("Content/sprites/characters/main.png", sf::Vector2i(64, 64));
	sprite->AddAnimation("stopped_up", { { 0, 0 }, { 0, 0 }, walkFrameDuration });
	sprite->AddAnimation("stopped_down", { { 0, 64 * 2 }, { 0, 64 * 2 }, walkFrameDuration });
	sprite->AddAnimation("stopped_left", { { 0, 64 * 1 }, { 0, 64 * 1 }, walkFrameDuration });
	sprite->AddAnimation("stopped_right", { { 0, 64 * 3 }, { 0, 64 * 3 }, walkFrameDuration });

	sprite->AddAnimation("walking_up", { { 64, 64 * 0 }, { 64 * 8, 64 * 0 }, walkFrameDuration });
	sprite->AddAnimation("walking_down", { { 64, 64 * 2 }, { 64 * 8, 64 * 2 }, walkFrameDuration });
	sprite->AddAnimation("walking_left", { { 64, 64 * 1 }, { 64 * 8, 64 * 1 }, walkFrameDuration });
	sprite->AddAnimation("walking_right", { { 64, 64 * 3 }, { 64 * 8, 64 * 3 }, walkFrameDuration });

	sprite->AddAnimation("dying", { { 0, 64 * 12 }, { 64 * 5, 64 * 12 }, walkFrameDuration });
	Sprite = sprite;

	Health = Container<int>(8, 8);
	Arrows = Container<int>(50, 50);
	Magic = Container<int>(10, 10);
}


qfcgame::Hero::~Hero()
{
	delete Sprite;
}
