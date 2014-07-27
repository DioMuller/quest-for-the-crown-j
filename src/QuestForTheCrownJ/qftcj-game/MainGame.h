#pragma once
#include "Game.h"
#include "StructBase.h"
#include "ClientChannel.h"

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
			MainGame();

			void Connect(int localPort, std::string auth_token);

			~MainGame();

			void RefreshSceneFromServer();
			void SetEntities(std::vector<ServerEntityInfo> entities);

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			void Update(double dt);
	};
}