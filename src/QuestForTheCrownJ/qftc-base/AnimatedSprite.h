#pragma once

#include <SFML/Graphics.hpp>
#include "Definitions.h"

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
			// Animation
			std::string currentAnimation;
			sf::Vector2i frameCount;
			sf::Vector2i frameSize;
			sf::Vector2i currentFrame;
			double current_interval;
			// Animation list
			std::map<std::string, Animation> animations;

		/////////////////////////////////////
		// Properties
		/////////////////////////////////////
		public:
			getdecl(sf::Vector2f, Center);
			getdecl(sf::Vector2f, Feet);
			getdecl(sf::FloatRect, Area);
			getsetdecl(std::string, CurrentAnimation);
			// Padding: width = margin-right, height = margin-bottom
			autoprop(public, public, sf::FloatRect, padding, Padding);
			autoprop(public, public, sf::Vector2f, position, Position);

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
			void Draw(sf::RenderWindow* window);
			void Update(double deltaTime);
			void Move(const sf::Vector2f& offset);
			bool Overlaps(const AnimatedSprite& other);

			std::string GetCurrentAnimation() { return currentAnimation; }
			void SetCurrentAnimation(std::string);
			sf::Vector2f GetCenter() { return position + sf::Vector2f(static_cast<float>(frameSize.x) / 2, static_cast<float>(frameSize.y) / 2); }
			sf::Vector2f GetFeet() { return position + sf::Vector2f(static_cast<float>(frameSize.x) / 2, static_cast<float>(frameSize.y - padding.height)); }
			sf::FloatRect GetArea() const;

			void AddAnimation(std::string key, Animation animation);
	};
}
