#pragma once
#include <stdexcept>

namespace qfcserver {

	class SockError : public std::runtime_error
	{
		public:
			SockError(const std::string& msg) : std::runtime_error(msg) { }
	};

	class SockBindError : public SockError
	{
	public:
		SockBindError(const std::string& msg) : SockError(msg) { }
	};
}