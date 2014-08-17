#pragma once
#include "BattleScene.h"
#include "Entity.h"

namespace qfcserver
{
	class ServerBattle : public qfcbase::BattleScene
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::vector<qfcbase::Turn> receivedTurns;
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			ServerBattle(std::weak_ptr<qfcbase::Game> parent);
			~ServerBattle();
		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual bool SelectAction(std::shared_ptr<qfcbase::BattleEntity> entity);

			void SendTurn(qfcbase::BattleAction command, int entity, int target, int additional_info, std::shared_ptr<qfcbase::Entity> currentEntity, int turn_id = -1);
			void ReceiveTurn(int turn_id, qfcbase::BattleAction command, int additional_info, int entity_id, int target_id);
			void SendTurnToClients(int turn_id);
	};
}

