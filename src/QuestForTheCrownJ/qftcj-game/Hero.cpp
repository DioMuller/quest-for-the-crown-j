#include "Hero.h"
#include "AnimatedSprite.h"
#include "Container.h"

qfcgame::Hero::Hero()
{
	const double walkFrameDuration = 0.1;

	auto sprite = new qfcbase::AnimatedSprite("Content/sprites/characters/main.png", sf::Vector2i(64, 64));
	sprite->AddAnimation("stopped_up", { { 0, 0 }, { 0, 0 }, walkFrameDuration });
	sprite->AddAnimation("stopped_down", { { 0, 2 }, { 0, 2 }, walkFrameDuration });
	sprite->AddAnimation("stopped_left", { { 0, 1 }, { 0, 1 }, walkFrameDuration });
	sprite->AddAnimation("stopped_right", { { 0, 3 }, { 0, 3 }, walkFrameDuration });

	sprite->AddAnimation("walking_up", { { 1, 0 }, { 8, 0 }, walkFrameDuration });
	sprite->AddAnimation("walking_down", { { 1, 2 }, { 8, 2 }, walkFrameDuration });
	sprite->AddAnimation("walking_left", { { 1, 1 }, { 8, 1 }, walkFrameDuration });
	sprite->AddAnimation("walking_right", { { 1, 3 }, { 8, 3 }, walkFrameDuration });

	sprite->AddAnimation("dying", { { 0, 12 }, { 5, 12 }, walkFrameDuration });
	Sprite = sprite;
}


qfcgame::Hero::~Hero()
{
	delete Sprite;
}
