#pragma once
#include "ServerStructs.h"
#include <string>
static class NetHelper
{
	public:
		static std::string DecodeAnimation(int info);
		static int EncodeAnimation(std::string animation);

		static EntityCategory EncodeCategory(std::string category);
		static std::string DecodeCategory(EntityCategory category);
};
