#pragma once
#include <SFML\Graphics\RenderWindow.hpp>

namespace qfcbase
{
	class IDrawable
	{
	public:
		virtual void Draw(sf::RenderWindow* renderer) = 0;
	};

	class IUpdateable
	{
	public:
		virtual void Update(double delta) = 0;
	};

	class IRunnable:
		public IDrawable,
		public IUpdateable
	{
	};
}
