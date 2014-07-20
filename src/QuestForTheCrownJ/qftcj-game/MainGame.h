#pragma once
#include "Game.h"
#include "StructBase.h"
namespace qfcgame
{
	class MainGame : public qfcbase::Game
	{
		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			MainGame();
			~MainGame();

			void LoadScene(const PlayerInfo& player_info, std::vector<EntityInfo> entities_info);

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
	};
}