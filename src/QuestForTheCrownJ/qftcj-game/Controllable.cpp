#include "Controllable.h"
#include "Entity.h"

qfcgame::Controllable::Controllable(qfcbase::Entity* parent, PlayerInput* input)
	: qfcbase::Behavior(parent), input(input), lastDirection("down"), speed(16)
{
}


qfcgame::Controllable::~Controllable()
{
}

void qfcgame::Controllable::Update(double dt)
{
	input->Update(dt);

	std::string direction = "";
	if (input->MoveLeft() ^ input->MoveRight())
	{
		if (input->MoveLeft())
		{
			direction = "left";
			Parent->Sprite->Move({ (float)(-speed * dt) , 0});
		}
		else
		{
			direction = "right";
			Parent->Sprite->Move({ (float)(speed * dt), 0 });
		}
	}
	else if (input->MoveUp() ^ input->MoveDown())
	{
		if (input->MoveUp())
		{
			direction = "up";
			Parent->Sprite->Move({ 0, (float)(-speed * dt) });
		}
		else
		{
			direction = "down";
			Parent->Sprite->Move({ 0, (float)(speed * dt) });
		}
	}

	if (direction.empty())
		Parent->Sprite->CurrentAnimation = "stopped_" + lastDirection;
	else
	{
		Parent->Sprite->CurrentAnimation = "walking_" + direction;
		lastDirection = direction;
	}
}
