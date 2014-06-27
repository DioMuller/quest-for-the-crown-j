#include "Behavior.h"

using namespace qfcbase;

/////////////////////////////////////
// Constructors
/////////////////////////////////////
Behavior::Behavior(std::weak_ptr<Entity> parent)
{
	this->parent = parent;
}

Behavior::~Behavior()
{
}

/////////////////////////////////////
// Methods
/////////////////////////////////////
void Behavior::Update(double dt)
{

}