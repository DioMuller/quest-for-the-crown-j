#include "Controllable.h"
#include "Entity.h"

qfcgame::Controllable::Controllable(qfcbase::Entity* parent, PlayerInput* input)
	: qfcbase::Behavior(parent), input(input)
{
}


qfcgame::Controllable::~Controllable()
{
}

void qfcgame::Controllable::Update(double dt)
{
	input->Update(dt);
	Parent->Walk(input->Movement(), dt);
}
