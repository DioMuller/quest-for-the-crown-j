#pragma once
#include "BattleScene.h"
#include "Entity.h"

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

			void SendTurn(qfcbase::BattleAction command, int target, int additional_info, std::shared_ptr<qfcbase::Entity> currentEntity, int turn_id = -1);
			void ReceiveTurn(qfcbase::BattleAction command, int additional_info, int target_id);
			void SendTurnToClients(int turn_id);
	};
}

