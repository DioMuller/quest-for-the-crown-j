#pragma once
#include <WinSock2.h>

#include "ServerChannel.h"

namespace qfcserver {
	class Server
	{
		public:
			Server(int port);
			~Server();

			void Run();

		private:
			qfcnet::ServerChannel channel;
			void UpdateLoop();

			LauncherLoginResponse HandleLoginInfo(const LauncherLoginInfo& login_info);
	};
}
