#pragma once
#include "BattleScene.h"

#define REQUEST_INTERVAL 1.0

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
			sf::Text battleCommands;
			int lastTurn;
			double requestInterval;
			std::vector<qfcbase::Turn> receivedTurns;
			sf::Texture backgroundTexture;
			sf::Sprite backgroundImage;
			sf::Texture messageBoxTexture;
			sf::Sprite messageBoxImage;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			ClientBattle(std::weak_ptr<qfcbase::Game> parent, std::string background);
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
			void SendTurn(qfcbase::BattleAction command, int entity, int target, int additional_info);

			void ReceiveTurn(qfcbase::BattleAction command, int turn_id, int additional_info, int entity_id, int target_id);
	};
}