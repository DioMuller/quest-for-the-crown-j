#pragma once
#include "BattleScene.h"

namespace qfcgame
{
	class ClientBattle : public qfcbase::BattleScene
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			double time;
			double lastAttack;
			qfcbase::BattleAction playerAction;
			std::deque<std::string> lastMessages;
			sf::Text text;
			int lastTurn;
			std::vector<qfcbase::Turn> receivedTurns;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			ClientBattle(std::weak_ptr<qfcbase::Game> parent);
			~ClientBattle();
		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
			void Draw(sf::RenderWindow* renderer);
			
			bool SelectAction(std::shared_ptr<qfcbase::BattleEntity> entity);
			void PrintMessage(std::string message);

			void RequestTurn();
			void SendTurn(qfcbase::BattleAction command, int target, int additional_info);

			void ReceiveTurn(qfcbase::BattleAction command, int turn_id, int additional_info, int target_id);
	};
}