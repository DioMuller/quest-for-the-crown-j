#include "ServerLevel.h"

using namespace qfcserver;
using namespace qfcbase;

ServerLevel::ServerLevel(std::weak_ptr<qfcbase::Game> game, int id)
	: Level(game, id)
{

}


ServerLevel::~ServerLevel()
{
}
