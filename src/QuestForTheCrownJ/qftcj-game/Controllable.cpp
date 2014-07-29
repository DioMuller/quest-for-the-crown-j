#include "Controllable.h"
#include "Entity.h"
#include "Level.h"
#include "Game.h"

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

		auto scene = std::dynamic_pointer_cast<qfcbase::Level>(parent->Scene().lock());

		if (scene)
		{
			qfcbase::Direction direction = qfcbase::Direction::NONE;

			if (parent->Sprite->Center.x < 0) direction = qfcbase::Direction::WEST;
			if (parent->Sprite->Center.x > (scene->Map()->GetMapSize().x)) direction = qfcbase::Direction::EAST;
			if (parent->Sprite->Center.y < 0) direction = qfcbase::Direction::NORTH;
			if (parent->Sprite->Center.y > (scene->Map()->GetMapSize().y)) direction = qfcbase::Direction::SOUTH;

			if (direction != qfcbase::Direction::NONE)
			{
				if (auto game = scene->GetParent().lock())
					game->GoToNeighbour(parent, direction);
			}
		}
	}
}
