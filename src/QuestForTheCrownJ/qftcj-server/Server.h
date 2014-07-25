#pragma once
#include <WinSock2.h>

#include "sqlite3.h"
#include "ServerChannel.h"

namespace qfcserver {
	class Server
	{
		public:
			Server(int port);
			~Server();

			void Run();

		private:
			sqlite3* db;
			qfcnet::ServerChannel channel;
			void UpdateLoop();

			LauncherLoginResponse HandleLoginInfo(const LauncherLoginInfo& login_info);
	};
}
