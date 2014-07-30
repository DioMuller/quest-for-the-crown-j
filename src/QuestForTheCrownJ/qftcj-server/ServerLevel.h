#pragma once
#include "Level.h"

namespace qfcserver
{
	class ServerLevel :
		public qfcbase::Level
	{
	public:
		ServerLevel(std::weak_ptr<qfcbase::Game> game, int id);
		~ServerLevel();
	private:
		//std::vector<int> players;
	};
}
