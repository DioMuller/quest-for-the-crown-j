#include "MainGame.h"

#include <memory>

#include "AudioPlayer.h"
#include "BattleScene.h"

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
		currentScene->Draw(renderer);
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

	currentScene = s;
	currentScene->AddRemoveEntities();
}

void MainGame::UnstackScene(std::shared_ptr<qfcbase::Entity> entity)
{
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
	auto battle = std::make_shared<qfcbase::BattleScene>(e1Scene->GetParent());

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

//void Scene::UnloadScene()
//{
//	parent->UnstackScene();
//}
