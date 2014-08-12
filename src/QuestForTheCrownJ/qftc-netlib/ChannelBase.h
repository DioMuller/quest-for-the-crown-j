#pragma once
#include <functional>
#include <string>
#include "Log.h"
namespace qfcnet {

	class ChannelBase
	{
	public:
		ChannelBase();
		~ChannelBase();

	protected:
		template <typename T>
		void HandlePacket(std::string package_type, std::function<void(T&)> handler, void* buffer, int size)
		{
			if (size != sizeof(T))
			{
				qfcbase::Log::Error((std::string)"Invalid packet size for " + package_type + ": " + std::to_string(size));
				return;
			}
			if (!handler)
			{
				qfcbase::Log::Debug("No handler for " + package_type);
				return;
			}

			auto info = (T*)buffer;
			handler(*info);
		}
	};
}
