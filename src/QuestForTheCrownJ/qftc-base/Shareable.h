#pragma once
template <typename T>
class Shareable
	: std::enable_shared_from_this<T>
{
public:
	std::shared_ptr<T> getptr()
	{
		return shared_from_this();
	}
};

