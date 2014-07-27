#include "BattleScene.h"
#include "Game.h"
#include "AudioPlayer.h"
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
	currentTurn = 0;

	AudioPlayer::PlayBGM("Firebrand");
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
	// Camera
	sf::Vector2f screenSize = sf::Vector2f(renderer->getSize().x, renderer->getSize().y);
	sf::Vector2f cameraPosition = sf::Vector2f(screenSize.x / 2, screenSize.y / 2);
	renderer->setView(sf::View(cameraPosition, screenSize));

	// Text
	std::ostringstream strs;
	strs << "Battle Time " << (int) time;
	std::string str = strs.str();

	text.setString(str);

	renderer->draw(text);
}