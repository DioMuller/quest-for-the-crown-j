#pragma once
#include <string>
#include <memory>
#include "Behavior.h"
#include "PlayerInput.h"

namespace qfcgame
{
	class Controllable :
		public qfcbase::Behavior
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::shared_ptr<PlayerInput> input;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Controllable(std::weak_ptr<qfcbase::Entity> parent, std::shared_ptr<PlayerInput> input);
			~Controllable();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);

		private:
			void UpdateAnimation();
	};
}
