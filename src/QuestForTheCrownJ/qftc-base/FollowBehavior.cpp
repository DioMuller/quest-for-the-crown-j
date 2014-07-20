#include "FollowBehavior.h"
#include "Entity.h"
#include "SFHelper.h"
#include <algorithm>

qfcbase::FollowBehavior::FollowBehavior(std::weak_ptr<qfcbase::Entity> parent, std::string targetCategory, float desiredDistance, float viewDistance)
	: qfcbase::Behavior(parent), targetCategory(targetCategory), desiredDistance(desiredDistance), viewDistance(viewDistance)
{
}


qfcbase::FollowBehavior::~FollowBehavior()
{
}

void qfcbase::FollowBehavior::Update(double dt)
{
	auto parent = Parent.lock();
	if (!parent) return;

	float targetDistance = -1;

	if (currentTarget.expired())
	{
		auto ents = parent->scene->GetEntities(targetCategory);
		std::shared_ptr<qfcbase::Entity> bestTarget = nullptr;
		auto position = parent->Sprite->Position;
		for (const auto& e : ents)
		{
			float eDist = qfcbase::SFHelper::length(e->Sprite->Position - position);

			if (eDist > viewDistance)
				continue;

			if (bestTarget == nullptr || targetDistance > eDist)
			{
				targetDistance = eDist;
				bestTarget = e;
			}
		}
		currentTarget = bestTarget;
	}

	auto target = currentTarget.lock();
	if (!target) return;

	auto direction = target->Sprite->Feet - parent->Sprite->Feet;

	if (targetDistance < 0)
		targetDistance = qfcbase::SFHelper::length(direction);

	direction = qfcbase::SFHelper::normalize(direction);

	if (targetDistance > viewDistance)
	{
		currentTarget.reset();
		return;
	}

	if (targetDistance > desiredDistance)
		parent->Walk(direction, dt);
}
