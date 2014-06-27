#include "Controllable.h"
#include "Entity.h"

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
	}
}
