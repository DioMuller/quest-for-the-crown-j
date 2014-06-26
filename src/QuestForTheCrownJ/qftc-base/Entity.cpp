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
	{
		Behavior* b = behaviors[behaviors.size() - 1];
		behaviors.pop_back();
		delete b;
	}
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Entity::Update(double dt)
{
	for (Behavior* b : behaviors)
	{
		b->Update(dt);
	}

	sprite->Update(dt);
}

void Entity::Draw(sf::RenderWindow* renderer)
{
	sprite->Draw(renderer);
}

void Entity::AddBehavior(Behavior* b)
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
