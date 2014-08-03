#include "Hero.h"

#include <memory>
#include <math.h>

#include "AnimatedSprite.h"
#include "Container.h"
#include "Level.h"
#include "Game.h"

using namespace qfcbase;

Hero::Hero()
{
	Speed = 5 * 32;
	const double walkFrameDuration = 0.1;

	auto sprite = std::make_shared<AnimatedSprite>("Content/sprites/characters/main.png", sf::Vector2i(64, 64));
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

	this->status.base_hp = 50;
	this->status.base_mp = 10;
	this->status.base_attack = 5;
	this->status.base_defense = 5;
	this->status.base_magic = 5;
	this->status.base_defense_magic = 5;
	this->status.base_speed = 5;

	this->status.mul_hp = 10;
	this->status.mul_mp = 2;
	this->status.mul_attack = 1;
	this->status.mul_defense = 1;
	this->status.mul_magic = 1;
	this->status.mul_defense_magic = 1;
	this->status.mul_speed = 1;

	sprite->Padding = sf::FloatRect(22, 32, 22, 2);
}


Hero::~Hero()
{
}


void Hero::Draw(sf::RenderWindow* renderer)
{
	sf::Vector2f cameraPosition = Sprite->Position;
	sf::Vector2f screenSize = sf::Vector2f(renderer->getSize().x, renderer->getSize().y);
	sf::Vector2u mapSize = (dynamic_cast<qfcbase::Level*>(scene.lock().get()))->Map()->GetMapSize();

	// Not on the same line because... It won't work. Not sure why.
	cameraPosition.x = max(cameraPosition.x, screenSize.x / 2);
	cameraPosition.x = min(cameraPosition.x, mapSize.x - screenSize.x /2);
	cameraPosition.y = max(cameraPosition.y, screenSize.y / 2);
	cameraPosition.y = min(cameraPosition.y, mapSize.y - screenSize.y / 2);

	renderer->setView(sf::View(cameraPosition, screenSize ));
	Entity::Draw(renderer);
}

void Hero::CollideWith(std::shared_ptr<Entity> e)
{
	if (e->category == "Enemy")
	{
		auto sceneInst = scene.lock();
		if (!sceneInst) return;

		sceneInst->AbortUpdate();

		auto game = sceneInst->GetParent().lock();
		if (!game) return;

		game->StartConfront(getptr(), e);
	}
}