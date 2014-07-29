#include "Entity.h"
#include "Level.h"
#include "SFHelper.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Entity::Entity()
	: status({}), speed(32), lastDirectionName("down")
{
}


Entity::~Entity()
{
	while (!behaviors.empty())
		behaviors.pop_back();
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Entity::Update(double dt)
{
	for (const auto& b : behaviors)
	{
		b->Update(dt);
	}

	sprite->Update(dt);
}

void Entity::Draw(sf::RenderWindow* renderer)
{
	sprite->Draw(renderer);
}

void Entity::AddBehavior(std::shared_ptr<Behavior> b)
{
	behaviors.push_back(b);
}

void Entity::Walk(sf::Vector2f direction, double dt)
{
	auto offset = sf::Vector2f((float)(speed * direction.x * dt), (float)(speed * direction.y * dt));
	if (!ValidPosition(offset))
	{
		auto check = sf::Vector2f(offset.x, 0);
		if (ValidPosition(check))
			offset = check;
		else
		{
			check = sf::Vector2f(0, offset.y);
			if (ValidPosition(check))
				offset = check;
			else
				return;
		}
	}

	std::string directionName = "";
	auto oldPos = Sprite->Position;
	Sprite->Move(offset);

	if (abs(direction.x) > abs(direction.y))
		directionName = direction.x < 0 ? "left" : "right";
	else if (direction.y)
		directionName = direction.y < 0 ? "up" : "down";

	if (directionName.empty())
		Sprite->CurrentAnimation = "stopped_" + lastDirectionName;
	else
	{
		Sprite->CurrentAnimation = "walking_" + directionName;
		lastDirectionName = directionName;
	}
}

bool Entity::ValidPosition(sf::Vector2f offset)
{
	auto sceneInst = scene.lock();
	if (!sceneInst) return false;

	auto area = SFHelper::translate(Sprite->GetArea(), offset);

	/* Level Collision */
	auto level = std::dynamic_pointer_cast<Level>(sceneInst);

	if ( level )
	{
		auto map = level->Map();

		for (auto layer : map->GetLayers())
		{
			if (layer.name == "Collision")
			{
				//TODO: Collision?
				for (auto object : layer.objects)
				{
					sf::Vector2f position = sf::Vector2f(area.left, area.top);
					sf::Vector2f size = sf::Vector2f(area.width, area.height);

					if (object.Contains(position) || object.Contains(position + size) ||
						object.Contains(position + sf::Vector2f(size.x, 0)) ||
						object.Contains(position + sf::Vector2f(0, size.y)))
					{
						return false;
					}
				}
			}
		}
	}
	/* End Level Collision */

	auto entCollisions = sceneInst->GetEntities([this, area](std::shared_ptr<Entity> e){
		if (this == e.get())
			return false;
		return  e->Sprite->GetArea().intersects(area);
	});

	for (auto e : entCollisions)
	{
		this->CollideWith(e);
	}

	return entCollisions.size() <= 0;
}

void Entity::CollideWith(std::shared_ptr<Entity> e) {}