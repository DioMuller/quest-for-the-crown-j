#include "GameEntityFactory.h"
#include "Hero.h"
#include "Slime.h"
#include "KeyboardInput.h"
#include "FollowBehavior.h"
#include "Controllable.h"
#include <queue>

using namespace qfcbase;

std::shared_ptr<Entity> GameEntityFactory::Player;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
GameEntityFactory::GameEntityFactory() : EntityFactory()
{
}


GameEntityFactory::~GameEntityFactory()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
std::shared_ptr<qfcbase::Entity> GameEntityFactory::GenerateEntity(std::weak_ptr<qfcbase::Scene> scene, std::string type, tmx::MapObject object)
{
	std::vector<std::function<std::shared_ptr<Behavior>(std::weak_ptr<Entity>)>> additionalBehaviors;

	if (type == "Player")
	{
		if (Player) return nullptr;
		additionalBehaviors.push_back([](std::weak_ptr<Entity> e) {
			return std::make_shared<qfcgame::Controllable>(e, std::make_shared<qfcgame::KeyboardInput>());
		});
	}

	auto ent = EntityFactory::GenerateEntity(scene, type, object);
	for (auto b : additionalBehaviors)
		ent->AddBehavior(b(ent));

	if (type == "Player")
		Player = ent;

	return ent;
}