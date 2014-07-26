#pragma once

template <typename T, typename TF>
std::thread async_promise(std::promise<T>& promise, TF f)
{
	return std::thread([&]() {
		sync_promise(promise, f);
	});
}

template <typename T, typename TF>
void sync_promise(std::promise<T>& promise, TF f)
{
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

template <typename T, typename TF>
std::thread async_on_promise(std::promise<T>& promise, TF f)
{
	return std::thread([&]() {
		sync_on_promise(promise, f);
	});
}

template <typename T, typename TF>
void sync_on_promise(std::promise<T>& promise, TF f)
{
	try
	{
		f();
	}
	catch (std::exception& ex)
	{
		try {
			promise.set_exception(std::current_exception());
		}
		catch (...) {}
	}
}