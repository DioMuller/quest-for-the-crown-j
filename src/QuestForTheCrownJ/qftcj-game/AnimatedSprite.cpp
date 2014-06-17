#include "AnimatedSprite.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
AnimatedSprite::AnimatedSprite(std::string path, sf::Vector2i frameSize, sf::Vector2f position)
{
	if (!texture.loadFromFile(path))
	{
		// TODO: Log.
	}

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


AnimatedSprite::~AnimatedSprite()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void AnimatedSprite::Render(sf::RenderWindow* window)
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

}