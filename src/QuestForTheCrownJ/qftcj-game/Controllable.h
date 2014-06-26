#pragma once
#include <string>
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
			PlayerInput* input;
			std::string lastDirection;
		public:
			float speed;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			Controllable(qfcbase::Entity* parent, PlayerInput* input);
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
