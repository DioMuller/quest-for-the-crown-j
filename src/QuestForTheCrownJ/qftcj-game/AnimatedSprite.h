#pragma once

#include <SFML/Graphics.hpp>

namespace qfcbase
{
	typedef struct struct_animation
	{
		sf::Vector2i firstFrame;
		sf::Vector2i lastFrame;
		double frameInterval;
	} Animation;


	class AnimatedSprite
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			// Texture
			sf::Texture texture;
			sf::Sprite sprite;
			// Position
			sf::Vector2f position;
			// Animation
			sf::Vector2i frameCount;
			sf::Vector2i frameSize;
			sf::Vector2i currentFrame;
			double current_interval;
			// Animation list
			std::map<std::string, Animation> animations;
			std::string currentAnimation;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			AnimatedSprite(std::string path, sf::Vector2i frameSize, sf::Vector2f position = sf::Vector2f(0, 0));
			~AnimatedSprite();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Render(sf::RenderWindow* window);
			void Update(double deltaTime);

			void AddAnimation(std::string key, Animation animation);
			void SetCurrentAnimation(std::string animationKey);
	};
}
