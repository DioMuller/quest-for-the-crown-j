#include "FollowBehavior.h"
#include "Entity.h"

qfcgame::FollowBehavior::FollowBehavior(qfcbase::Entity* parent)
	: qfcbase::Behavior(parent)
{
}


qfcgame::FollowBehavior::~FollowBehavior()
{
}

void qfcgame::FollowBehavior::Update(double dt)
{
	//auto target = Parent->
	//Parent->Walk(, dt);
}
