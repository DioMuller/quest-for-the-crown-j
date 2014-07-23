#pragma once
#include <ppltasks.h>
#include "Game.h"
#include "StructBase.h"
#include "ClientChannel.h"
#include <pplcancellation_token.h>

namespace qfcgame
{
	class MainGame : public qfcbase::Game
	{
		private:
			qfcnet::ClientChannel clientChannel;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			MainGame(std::string auth_token);
			~MainGame();

			void RefreshSceneFromServer();
			void SetEntities(std::vector<EntityInfo> entities);

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
	};
}