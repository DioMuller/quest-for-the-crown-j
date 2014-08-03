#pragma once

#include <functional>
#include <SFML\System\Vector2.hpp>

#include "Behavior.h"

namespace qfcbase
{
	class WatchPosition :
		public qfcbase::Behavior
	{
	private:
		double current_time, min_time;
		std::function<void(std::shared_ptr<Entity>)> onMove;

	public:
		WatchPosition::WatchPosition(std::weak_ptr<qfcbase::Entity> parent, std::function<void(std::shared_ptr<Entity>)> onMove, double minTime);
		~WatchPosition();

		void WatchPosition::Update(double dt);
	};
}
