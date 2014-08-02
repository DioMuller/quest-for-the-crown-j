#include "ServerEntityFactory.h"

#include "WatchPosition.h"
#include "Level.h"
#include "ServerHelper.h"
#include "Definitions.h"

using namespace qfcserver;
using namespace qfcbase;
using namespace qfcnet;

ServerEntityFactory::ServerEntityFactory(std::weak_ptr<ServerChannel> channel)
{
	this->channel = channel;
}


ServerEntityFactory::~ServerEntityFactory()
{
}


