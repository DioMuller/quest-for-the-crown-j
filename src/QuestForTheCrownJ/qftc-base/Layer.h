#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
namespace qfcbase
{
	class Layer
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			int** data;
			std::string name;
			sf::Vector2i size;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Layer(std::string name, sf::Vector2i size, std::string csvData);
			~Layer();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			int GetData(int x, int y);
	};

}