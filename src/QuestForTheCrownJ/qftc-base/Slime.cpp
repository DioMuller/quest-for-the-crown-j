#include "Slime.h"

qfcbase::Slime::Slime()
{
	Speed = 1 * 32;
	const double stoppedFrameDuration = 0.3;
	const double walkFrameDuration = 0.1;

	auto sprite = std::make_shared<qfcbase::AnimatedSprite>("Content/sprites/characters/slime.png", sf::Vector2i(32, 42));
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
	sprite->Padding = sf::FloatRect(6, 14, 6, 14);

	this->status.base_hp = 10;
	this->status.base_mp = 1;
	this->status.base_attack = 2;
	this->status.base_defense = 2;
	this->status.base_magic = 2;
	this->status.base_defense_magic = 2;
	this->status.base_speed = 2;

	this->status.mul_hp = 3;
	this->status.mul_mp = 1;
	this->status.mul_attack = 1;
	this->status.mul_defense = 1;
	this->status.mul_magic = 1;
	this->status.mul_defense_magic = 1;
	this->status.mul_speed = 1;
}


qfcbase::Slime::~Slime()
{
}
