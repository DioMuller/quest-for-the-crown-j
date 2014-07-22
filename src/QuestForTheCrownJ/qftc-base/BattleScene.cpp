#include "BattleScene.h"
#include "Game.h"
#include <string>
#include <sstream>

using namespace qfcbase;

BattleScene::BattleScene(Game* parent) : Scene(parent)
{
	time = 0.0;
	font.loadFromFile("Content/fonts/8bitwonder.ttf");

	text = sf::Text("Battle!", font);
	text.setCharacterSize(12);
	text.setPosition(10, 10);
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
	std::ostringstream strs;
	strs << "Battle Time " << (int) time;
	std::string str = strs.str();

	text.setString(str);

	renderer->draw(text);
}