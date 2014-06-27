#include "Entity.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Entity::Entity()
	: status({}), speed(32), lastDirectionName("down")
{
}


Entity::~Entity()
{
	while (!behaviors.empty())
		behaviors.pop_back();
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Entity::Update(double dt)
{
	for (const auto& b : behaviors)
	{
		b->Update(dt);
	}

	sprite->Update(dt);
}

void Entity::Draw(sf::RenderWindow* renderer)
{
	sprite->Draw(renderer);
}

void Entity::AddBehavior(std::shared_ptr<Behavior> b)
{
	behaviors.push_back(b);
}

void Entity::Walk(sf::Vector2f direction, double dt)
{
	std::string directionName = "";

	Sprite->Move({ (float)(speed * direction.x * dt), (float)(speed * direction.y * dt) });

	if (abs(direction.x) > abs(direction.y))
		directionName = direction.x < 0 ? "left" : "right";
	else if (direction.y)
		directionName = direction.y < 0 ? "up" : "down";

	if (directionName.empty())
		Sprite->CurrentAnimation = "stopped_" + lastDirectionName;
	else
	{
		Sprite->CurrentAnimation = "walking_" + directionName;
		lastDirectionName = directionName;
	}
}
