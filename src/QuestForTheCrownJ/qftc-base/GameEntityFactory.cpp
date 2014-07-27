#include "GameEntityFactory.h"
#include "Hero.h"
#include "Slime.h"
#include "KeyboardInput.h"
#include "FollowBehavior.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
GameEntityFactory::GameEntityFactory() : EntityFactory()
{
	hasPlayer = false;
}


GameEntityFactory::~GameEntityFactory()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
std::shared_ptr<qfcbase::Entity> GameEntityFactory::GenerateEntity(qfcbase::Scene* scene, std::string type, tmx::MapObject object)
{
	std::shared_ptr<qfcbase::Entity> entity;

	if (type == "Enemy" )
	{
		entity = std::shared_ptr<qfcbase::Entity>(new Slime());
		entity->AddBehavior(std::make_shared<FollowBehavior>(entity, "GoodGuy", 5, 32 * 4));
		entity->category = "Enemy";
		entity->scene = scene;
	}
	else if (type == "Player" && !hasPlayer)
	{
		entity = std::shared_ptr<qfcbase::Entity>(new Hero());
		entity->category = "GoodGuy";
		entity->scene = scene;

		hasPlayer = true;
	}
	else
	{
		return nullptr;
	}

	entity->Sprite->Position = object.GetPosition();
	return entity;
}