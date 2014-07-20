#include "BattleScene.h"
#include "Game.h"

using namespace qfcbase;

BattleScene::BattleScene(Game* parent) : Scene(parent)
{
}


BattleScene::~BattleScene()
{
}

void BattleScene::Update(double dt)
{
	time += dt;

	if ( time > 5.0)
		parent->UnstackScene();
}

void BattleScene::Draw(sf::RenderWindow* renderer)
{

}