#include "WatchPosition.h"

#include "Entity.h"

using namespace qfcbase;

WatchPosition::WatchPosition(std::weak_ptr<qfcbase::Entity> parent, std::function<void(std::shared_ptr<Entity>)> onMove, double minTime)
	: Behavior(parent), current_time(0), min_time(minTime)
{
	this->onMove = onMove;
}


WatchPosition::~WatchPosition()
{
}

void WatchPosition::Update(double dt)
{
	current_time += dt;
	if (current_time > min_time)
	{
		current_time = 0;
		if (auto ent = GetParent().lock())
			onMove(ent);
	}
}
