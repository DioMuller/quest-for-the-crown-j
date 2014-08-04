#include "Walker.h"
#include "Entity.h"
#include "SFHelper.h"

using namespace qfcbase;
using namespace qfcgame;

Walker::Walker(std::weak_ptr<qfcbase::Entity> parent) : Behavior(parent)
{

}

Walker::~Walker()
{
}

void Update(double deltaTime)
{

}

void qfcgame::Walker::WalkTo(sf::Vector2f position)
{
	desired_position = std::make_shared<sf::Vector2f>(position);
}

void qfcgame::Walker::Update(double dt)
{
	if (!desired_position)
		return;

	auto entity = GetParent().lock();
	if (!entity)
		return;

	sf::Vector2f currentPosition = entity->Sprite->Position;
	auto direction = *desired_position - currentPosition;
	auto distance = SFHelper::length(direction);
	if (abs(distance) < 2)
	{
		entity->Sprite->Position = *desired_position;
		desired_position = nullptr;
		return;
	}

	float speedScale = distance / 50;

	speedScale = max(1, speedScale);

	direction = SFHelper::normalize(direction) * speedScale;

	if (distance < entity->Speed)
		entity->Walk(direction, dt);
	else
		entity->Sprite->Position = *desired_position;
}
