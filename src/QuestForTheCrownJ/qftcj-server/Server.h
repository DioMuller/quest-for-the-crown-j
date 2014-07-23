#pragma once
#include <WinSock2.h>

namespace qfcserver {
	class Server
	{
		public:
			Server();
			~Server();

			void Run(int port);

		private:
			SOCKET StartNetwork(int port);
			void ListenMessages(int port);
			void UpdateLoop();
	};
}
