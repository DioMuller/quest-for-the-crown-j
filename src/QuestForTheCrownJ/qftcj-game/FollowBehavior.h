#pragma once
#include "Behavior.h"
#include <string>

namespace qfcgame
{
	class FollowBehavior :
		public qfcbase::Behavior
	{
		/////////////////////////////////////
		// Attributes
		/////////////////////////////////////
		private:
			std::weak_ptr<qfcbase::Entity> currentTarget;
		public:
			float desiredDistance;
			float viewDistance;
			std::string targetCategory;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			FollowBehavior(std::weak_ptr<qfcbase::Entity> parent, std::string targetCategory, float desiredDistance, float viewDistance);
			~FollowBehavior();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
	};
}
