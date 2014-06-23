#include <exception>
#include "Layer.h"
#include "StringHelper.h"

using namespace qfcbase;

Layer::Layer(std::string name, sf::Vector2i size, std::string csvData)
{
	this->name = name;
	this->size = size;

	this->data = new int*[size.x];
	std::vector<std::string> separated = split(csvData, ',');

	if (separated.size() != (size.x * size.y)) throw std::exception("The csv data size is different from the Layer data size.");

	for (int i = 0; i < separated.size(); i++)
	{
		data[i % size.x][i / size.x] = std::stoi(separated[i]);
	}
}


Layer::~Layer()
{
}

int Layer::GetData(int x, int y)
{
	if (x < 0 || y < 0 || x >= this->size.x || y >= this->size.y) return -1;

	return this->data[x][y];
}