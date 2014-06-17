#include "Window.h"
#include <chrono>
#include <thread>

int main()
{
	qfcbase::Window window({ 800, 600 }, "Quest for the Crown J");

	window.Run();

	return 0;
}