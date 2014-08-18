#include "AnimatedSprite.h"
#include "Log.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
AnimatedSprite::AnimatedSprite(std::string path, sf::Vector2i frameSize, sf::Vector2f position)
{
	if (!texture.loadFromFile(path))
	{
		Log::Error("Error loading texture file: " + path);
	}
	else
	{
		sprite.setTexture(texture);

		// Position
		this->position = position;

		// Animation
		this->frameSize = frameSize;
		this->frameCount;
		this->currentFrame = sf::Vector2i(0, 0);
		this->current_interval = 0.0f;

		// Animation list
		animations["Default"] = { sf::Vector2i(0, 0), sf::Vector2i(3, 0), 60.0 };
		currentAnimation = "Default";
	}
}


AnimatedSprite::~AnimatedSprite()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void AnimatedSprite::Draw(sf::RenderWindow* window)
{
	sprite.setTextureRect(sf::IntRect(currentFrame.x * frameSize.x, currentFrame.y * frameSize.y, frameSize.x, frameSize.y));
	sprite.setPosition(position);
	window->draw(sprite);
}

void AnimatedSprite::Update(double deltaTime)
{
	current_interval += deltaTime;

	if (current_interval >= animations[currentAnimation].frameInterval)
	{
		currentFrame.x++;

		if (currentFrame.x > animations[currentAnimation].lastFrame.x)
		{
			currentFrame.x = animations[currentAnimation].firstFrame.x;

			currentFrame.y++;

			if (currentFrame.y > animations[currentAnimation].lastFrame.y)
			{
				currentFrame.y = animations[currentAnimation].firstFrame.y;
			}
		}

		current_interval -= animations[currentAnimation].frameInterval;
	}
}

void AnimatedSprite::AddAnimation(std::string key, Animation animation)
{
	animations[key] = animation;
}

void AnimatedSprite::SetCurrentAnimation(std::string animationKey)
{
	auto anim = animations[animationKey];

	if (currentAnimation == animationKey && !(currentFrame == anim.lastFrame && current_interval >= anim.frameInterval))
		return;

	currentAnimation = animationKey;
	this->currentFrame = anim.firstFrame;
	this->current_interval = 0;
}

void AnimatedSprite::Move(const sf::Vector2f& offset)
{
	position += offset;
}

bool AnimatedSprite::Overlaps(const AnimatedSprite& other)
{
	return GetArea().intersects(other.GetArea());
}

sf::FloatRect AnimatedSprite::GetArea() const
{
	return sf::FloatRect(
		position.x + padding.left,
		position.y + padding.top,
		frameSize.x - padding.width - padding.left,
		frameSize.y - padding.height - padding.top);
}

void AnimatedSprite::SetScale(float scale)
{
	sprite.setScale(scale, scale);
}