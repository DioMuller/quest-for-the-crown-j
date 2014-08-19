#include "Level.h"
#include "LevelCollection.h"
#include "LevelLoader.h"
#include "AudioPlayer.h"

using namespace qfcbase;

Level::Level(std::weak_ptr<Game> game, int id, bool playBGM) : Scene(game)
{
	this->id = id;
	this->playBGM = playBGM;
}


Level::~Level()
{
}


/////////////////////////////////////
// Methods
/////////////////////////////////////
void Level::Update(double dt)
{
	Scene::Update(dt);
}

void Level::Draw(sf::RenderWindow* renderer)
{
	// TODO: Replace camera vector.
	renderer->draw(*map);
	Scene::Draw(renderer);
}

void Level::GoToNeighbour(std::shared_ptr<Entity> entity, Direction direction)
{
	if (direction == Direction::EAST) entity->Sprite->Position = sf::Vector2f(entity->Sprite->Area.width, entity->Sprite->Position.y);
	else if (direction == Direction::WEST) entity->Sprite->Position = sf::Vector2f(map->GetMapSize().x - entity->Sprite->Area.width, entity->Sprite->Position.y);
	else if (direction == Direction::NORTH) entity->Sprite->Position = sf::Vector2f(entity->Sprite->Position.x, map->GetMapSize().y - entity->Sprite->Area.height);
	else if (direction == Direction::SOUTH) entity->Sprite->Position = sf::Vector2f(entity->Sprite->Position.x, entity->Sprite->Area.height);

	LoadMap(LevelCollection::GetNeighbour(id, direction)->mapFile);
	this->AddEntity(entity); // Since all entities are removed.
}

void Level::SetNeighbor(Direction direction, int neighborId)
{
	this->neighbors[(int)direction] = neighborId;
}

void Level::LoadMap(std::string tmxFile)
{
	if (tmxFile == "") return;

	this->map = std::make_shared<tmx::MapLoader>(tmxFile.substr(0, tmxFile.find_last_of("\\/")));
	this->map->AddSearchPath("Content/tilesets/"); // For tsx files.
	this->map->AddSearchPath("Content/tiles/"); // For png files.
	this->map->Load(tmxFile.substr(tmxFile.find_last_of("\\/"), tmxFile.length()));

	RemoveAllEntities();

	auto info = LevelCollection::GetLevel(tmxFile);

	if (info)
	{
		this->id = info->levelId;
		this->bgm = info->music;
		this->title = info->title;
		this->battleBackground = info->background;

		for (int i = 0; i < 4; i++)
		{
			SetNeighbor((Direction)i, info->neighbours[i]);
		}

		if(playBGM) AudioPlayer::PlayBGM(bgm);
	}

	auto layers = map->GetLayers();

	for (auto layer : layers)
	{
		if (layer.type == tmx::MapLayerType::ObjectGroup)
		{
			for (auto object : layer.objects)
			{
				std::shared_ptr<Entity> entity = LevelLoader::CreateEntity(getptr(), id, object);
				if (entity != nullptr) AddEntity(entity);
			}
		}
	}
}

void Level::OnResume()
{
	if (playBGM) AudioPlayer::PlayBGM(bgm);
}