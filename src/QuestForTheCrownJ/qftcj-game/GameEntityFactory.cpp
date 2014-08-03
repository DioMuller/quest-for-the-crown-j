#include "GameEntityFactory.h"

#include <queue>

#include "Hero.h"
#include "Slime.h"
#include "KeyboardInput.h"
#include "FollowBehavior.h"
#include "Controllable.h"
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

	if (type == "Player")
	{
		if (hasPlayer) return nullptr;
		additionalBehaviors.push_back([](std::weak_ptr<Entity> e) {
			return std::make_shared<Controllable>(e, std::make_shared<KeyboardInput>());
		});
	}

	auto ent = EntityFactory::GenerateEntity(scene, type, object);
	for (auto b : additionalBehaviors)
		ent->AddBehavior(b(ent));

	if (type == "Player")
	{
		hasPlayer = true;
		auto sceneInst = scene.lock();
		if (sceneInst)
		{
			auto gameInst = std::dynamic_pointer_cast<MainGame>(sceneInst->GetParent().lock());
			if (gameInst)
				gameInst->SetPlayer(ent);
		}
	}

	return ent;
}