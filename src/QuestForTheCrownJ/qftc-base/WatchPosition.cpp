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
	last_time += dt;

	if (current_time > 0)
		current_time -= dt;

	if (current_time > 0)
		return;

	auto ent = GetParent().lock();
	if (!ent)
		return;

	if (ent->Sprite->Position == last_position && ent->Sprite->CurrentAnimation == last_animation)
	{
		if (repeat_count >= max_repeat && last_time < force_send_every)
			return;
		repeat_count++;
	}
	else
	{
		repeat_count = 0;
	}

	onMove(ent);
	current_time = min_time;
	last_position = ent->Sprite->Position;
	last_animation = ent->Sprite->CurrentAnimation;
	last_time = 0;
}
