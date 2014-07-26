#pragma once

template <typename T, typename TF>
void on_promise(bool async, std::promise<T>& promise, TF f)
{
	if (async)
	{
		std::thread([&]() {
			on_promise(false, promise, f);
		}).detach();
		return;
	}

	try
	{
		f();
	}
	catch (std::exception& ex)
	{
		try {
			promise.set_exception(std::current_exception());
		}
		catch (...) { }
	}
}


template <typename T, typename TF>
void set_promise(bool async, std::promise<T>& promise, TF f)
{
	if (async)
	{
		std::thread([&]() {
			set_promise(false, promise, f);
		}).detach();
		return;
	}

	try
	{
		T res = f();
		try {
			promise.set_value(res);
		} catch (...) { }
	}
	catch (std::exception& ex)
	{
		try {
			promise.set_exception(std::current_exception());
		} catch (...) { }
	}
}
