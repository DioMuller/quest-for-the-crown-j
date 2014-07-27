#include "BattleEntity.h"
#include "GameAssets.h"
#include <string>
#include <sstream>

using namespace qfcbase;

BattleEntity::BattleEntity(std::shared_ptr<Entity> parent) : Entity()
{
	this->parent = parent;

	this->text = sf::Text("Info", *GameAssets::DefaultFont());
	text.setCharacterSize(12);
	text.setPosition(10, 10);
}


BattleEntity::~BattleEntity()
{
}

void BattleEntity::DrawEntity(sf::RenderWindow* renderer, sf::Vector2f position)
{
	sf::Vector2f pos = parent->Sprite->Position;
	parent->Sprite->SetPosition(position);
	parent->Sprite->Draw(renderer);
	parent->Sprite->SetPosition(pos);
}

void BattleEntity::DrawInfo(sf::RenderWindow* renderer, sf::Vector2f position)
{
	text.setPosition(position);

	std::ostringstream sstream;
	std::string str;

	sstream << "HP " << parent->status.HP << std::endl
			<< "MP " << parent->status.MP << std::endl
			<< "Potions " << 0;
	str = sstream.str();
	text.setString(str);
	renderer->draw(text);
}
