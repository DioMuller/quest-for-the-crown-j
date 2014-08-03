#include "MainGame.h"

#include <memory>

#include "AudioPlayer.h"
#include "BattleScene.h"
#include "GameEntityFactory.h"
#include "KeyboardInput.h"

using namespace qfcgame;
using namespace qfcbase;


MainGame::MainGame()
	: currentScene(nullptr)
{
	qfcbase::AudioPlayer::SetMusicPath("Content/bgm/");
	qfcbase::AudioPlayer::SetSoundPath("Content/sound/");
}

MainGame::~MainGame()
{
}

void MainGame::Update(double dt)
{
	if (currentScene != nullptr)
		currentScene->Update(dt);
}

void MainGame::Draw(sf::RenderWindow* renderer)
{
	if (currentScene != nullptr)
	{
		if (player)
			PositionCamera(renderer, player->Sprite->Position);

		currentScene->Draw(renderer);
	}
}

void MainGame::PositionCamera(sf::RenderWindow* renderer, sf::Vector2f cameraPosition)
{
	sf::Vector2f screenSize = sf::Vector2f(renderer->getSize().x, renderer->getSize().y);
	auto level = std::dynamic_pointer_cast<qfcbase::Level>(currentScene);

	sf::Vector2u mapSize;
	if (level)
		mapSize = level->Map()->GetMapSize();

	// Not on the same line because... It won't work. Not sure why.
	cameraPosition.x = max(cameraPosition.x, screenSize.x / 2);
	cameraPosition.x = min(cameraPosition.x, mapSize.x - screenSize.x / 2);
	cameraPosition.y = max(cameraPosition.y, screenSize.y / 2);
	cameraPosition.y = min(cameraPosition.y, mapSize.y - screenSize.y / 2);

	renderer->setView(sf::View(cameraPosition, screenSize));
}

/////////////////////////////////////
// Methods
/////////////////////////////////////

void MainGame::LoadScene(std::shared_ptr<Scene> s, bool stack)
{
	if (s == currentScene) return;

	if (stack)
	{
		pastScenes.push(currentScene);
	}

	if(currentScene) currentScene->AbortUpdate();

	currentScene = s;
}

void MainGame::UnstackScene(std::shared_ptr<qfcbase::Entity> entity)
{
	if (entity != player)
		return;

	if (pastScenes.size() > 0)
	{
		currentScene = (std::shared_ptr<Scene>)pastScenes.top();
		pastScenes.pop();
		currentScene->OnResume();
	}
}

bool MainGame::IsCurrentScene(std::shared_ptr<Scene> scene)
{
	return (currentScene == scene);
}

void MainGame::StartConfront(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	auto e1Scene = e1->scene.lock();
	if (!e1Scene)
		return;

	e1Scene->RemoveEntity(e2);
	auto battle = std::make_shared<qfcbase::BattleScene>(getptr());

	if (battle->PlayerJoin(e1) &&
		battle->AddMonster(e2))
		LoadScene(battle, true);
}

void qfcgame::MainGame::GoToNeighbour(std::shared_ptr<qfcbase::Entity> entity, qfcbase::Direction direction)
{
	// Hot-swap current level content.
	auto level = std::dynamic_pointer_cast<Level>(entity->scene.lock());
	level->GoToNeighbour(entity, direction);
}

void qfcgame::MainGame::SetPlayer(std::shared_ptr<qfcbase::Entity> player)
{
	if (this->player)
	{
		player->RemoveBehavior(controllable_behavior);
	}

	this->player = player;

	if (this->player)
	{
		if (!controllable_behavior)
			controllable_behavior = std::make_shared<Controllable>(this->player, std::make_shared<KeyboardInput>());
		else
			controllable_behavior->Parent = this->player;

		this->player->AddBehavior(controllable_behavior);
	}
}

//void Scene::UnloadScene()
//{
//	parent->UnstackScene();
//}
