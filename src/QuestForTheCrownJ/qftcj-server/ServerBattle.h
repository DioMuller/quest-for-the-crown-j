#pragma once
#include "BattleScene.h"

namespace qfcserver
{
	class ServerBattle : public qfcbase::BattleScene
	{
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			ServerBattle(std::weak_ptr<qfcbase::Game> parent);
			~ServerBattle();
		public:
			virtual bool SelectAction(std::shared_ptr<qfcbase::BattleEntity> entity);
	};
}

