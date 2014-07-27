#include "BattleEntity.h"

using namespace qfcbase;

BattleEntity::BattleEntity(std::shared_ptr<Entity> parent) : Entity()
{
	this->parent = parent;
}


BattleEntity::~BattleEntity()
{
}
