#include "Controllable.h"
#include "Entity.h"

qfcgame::Controllable::Controllable(qfcbase::Entity* parent, PlayerInput* input)
	: qfcbase::Behavior(parent), input(input), lastDirection("down"), speed(parent->Speed)
{
}


qfcgame::Controllable::~Controllable()
{
}

void qfcgame::Controllable::Update(double dt)
{
	input->Update(dt);

	std::string direction = "";
	auto move = input->Movement();
	Parent->Sprite->Move({ (float)(speed * move.x * dt), (float)(speed * move.y * dt) });

	if (abs(move.x) > abs(move.y))
		direction = move.x < 0 ? "left" : "right";
	else if (move.y)
		direction = move.y < 0 ? "up" : "down";

	if (direction.empty())
		Parent->Sprite->CurrentAnimation = "stopped_" + lastDirection;
	else
	{
		Parent->Sprite->CurrentAnimation = "walking_" + direction;
		lastDirection = direction;
	}
}
