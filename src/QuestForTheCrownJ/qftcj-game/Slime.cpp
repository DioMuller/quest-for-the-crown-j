#include "Slime.h"


qfcgame::Slime::Slime()
{
	Speed = 3 * 32;
	const double stoppedFrameDuration = 0.3;
	const double walkFrameDuration = 0.1;

	auto sprite = new qfcbase::AnimatedSprite("Content/sprites/characters/slime.png", sf::Vector2i(32, 42));
	sprite->AddAnimation("stopped_up", { { 0, 0 }, { 2, 0 }, stoppedFrameDuration });
	sprite->AddAnimation("stopped_down", { { 0, 2 }, { 2, 2 }, stoppedFrameDuration });
	sprite->AddAnimation("stopped_left", { { 0, 3 }, { 2, 3 }, stoppedFrameDuration });
	sprite->AddAnimation("stopped_right", { { 0, 1 }, { 2, 1 }, stoppedFrameDuration });

	sprite->AddAnimation("walking_up", { { 0, 0 }, { 2, 0 }, walkFrameDuration });
	sprite->AddAnimation("walking_down", { { 0, 2 }, { 2, 2 }, walkFrameDuration });
	sprite->AddAnimation("walking_left", { { 0, 3 }, { 2, 3 }, walkFrameDuration });
	sprite->AddAnimation("walking_right", { { 0, 1 }, { 2, 1 }, walkFrameDuration });
	sprite->CurrentAnimation = "stopped_down";
	Sprite = sprite;
}


qfcgame::Slime::~Slime()
{
}
