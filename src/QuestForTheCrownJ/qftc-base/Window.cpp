#include "Window.h"
#include "Definitions.h"

using namespace qfcbase;

std::shared_ptr<Window> Window::current;

Window::Window(sf::Vector2i size, std::string title)
{
	this->size = size;
	this->title = title;
	this->window = new sf::RenderWindow(sf::VideoMode(size.x, size.y), title);	
}


Window::~Window()
{
	delete window;
}

void Window::Run(std::shared_ptr<IRunnable> runnable)
{
	current = getptr();

	// Sets the game.
	this->running = runnable;

	//Initial update duration = 1 frame.
	double delta = SECONDS_PER_FRAME;

	while (window->isOpen())
	{
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();

		sf::Event windowEvent;
		while (window->pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
				window->close();
			else if (windowEvent.type == sf::Event::GainedFocus)
				focused = true;
			else if (windowEvent.type == sf::Event::LostFocus)
				focused = false;
		}

		window->clear();
		runnable->Update(delta);
		runnable->Draw(window);
		window->display();
		end = std::chrono::system_clock::now();

		delta = ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / (1000.0 * 1000.0);

		if (delta < SECONDS_PER_FRAME)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((long long)((SECONDS_PER_FRAME - delta) * (1000 * 1000))));
			delta = SECONDS_PER_FRAME;
		}
	}

	current = nullptr;
}

bool Window::IsFocused()
{
	return focused;
}

void Window::SetTitle(std::string title)
{
	window->setTitle(title);
}

std::shared_ptr<Window> qfcbase::Window::GetCurrent()
{
	return current;
}
