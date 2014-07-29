#pragma once
#include "MainGame.h"

namespace qfcgame
{
	class MultiplayerGame :
		public MainGame
	{
		////////////////////////////////////////
		// Attributes
		////////////////////////////////////////
	private:
		qfcnet::ClientChannel clientChannel;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
	public:
		MultiplayerGame();
		~MultiplayerGame();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		void Connect(int localPort, std::string auth_token);
		void RefreshSceneFromServer();
		//void SetEntities(std::vector<ServerEntityInfo> entities);
	};
}
