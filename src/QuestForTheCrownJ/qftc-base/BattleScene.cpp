#include "BattleScene.h"
#include "Game.h"

using namespace qfcbase;

BattleScene::BattleScene(Game* parent) : Scene(parent)
{
	time = 0.0;
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