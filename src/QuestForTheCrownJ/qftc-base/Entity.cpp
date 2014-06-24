#include "Entity.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Entity::Entity()
	: status({})
{
	status.level = 1;
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
}

void Entity::Draw(sf::RenderWindow* renderer)
{
	sprite->Draw(renderer);
}

void Entity::AddBehavior(Behavior* b)
{
	behaviors.push_back(b);
}