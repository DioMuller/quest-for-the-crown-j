#include "GameEntityFactory.h"

#include <queue>

#include "MainGame.h"

using namespace qfcbase;
using namespace qfcgame;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
GameEntityFactory::GameEntityFactory() : EntityFactory(), hasPlayer(false)
{
}


GameEntityFactory::~GameEntityFactory()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
std::shared_ptr<Entity> GameEntityFactory::GenerateEntity(std::weak_ptr<Scene> scene, std::string type, tmx::MapObject object)
{
	std::vector<std::function<std::shared_ptr<Behavior>(std::weak_ptr<Entity>)>> additionalBehaviors;

	if (hasPlayer) return nullptr;

	auto ent = EntityFactory::GenerateEntity(scene, type, object);

	if (type == "Player")
	{
		hasPlayer = true;
		auto sceneInst = scene.lock();
		if (sceneInst)
		{
			if (auto gameInst = std::dynamic_pointer_cast<MainGame>(sceneInst->GetParent().lock()))
				gameInst->SetPlayer(ent);
		}
	}

	return ent;
}