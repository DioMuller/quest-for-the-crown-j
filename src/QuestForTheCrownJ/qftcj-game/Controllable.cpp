#include "Controllable.h"
#include "Entity.h"
#include "Level.h"

qfcgame::Controllable::Controllable(std::weak_ptr<qfcbase::Entity> parent, std::shared_ptr<PlayerInput> input)
	: qfcbase::Behavior(parent), input(input)
{
}


qfcgame::Controllable::~Controllable()
{
}

void qfcgame::Controllable::Update(double dt)
{
	if (auto parent = Parent.lock())
	{
		input->Update(dt);
		parent->Walk(input->Movement(), dt);

		qfcbase::Level* scene = (qfcbase::Level*)parent->Scene();

		if (scene)
		{
			qfcbase::Direction direction = qfcbase::Direction::NONE;

			if (parent->Sprite->Position.x < 0) direction = qfcbase::Direction::WEST;
			if (parent->Sprite->Position.x > scene->Map()->GetMapSize().x) direction = qfcbase::Direction::EAST;
			if (parent->Sprite->Position.y < 0) direction = qfcbase::Direction::NORTH;
			if (parent->Sprite->Position.y > scene->Map()->GetMapSize().y) direction = qfcbase::Direction::SOUTH;

			if (direction != qfcbase::Direction::NONE)
			{
				scene->GoToNeighbour(parent, direction);
			}
		}
	}
}
