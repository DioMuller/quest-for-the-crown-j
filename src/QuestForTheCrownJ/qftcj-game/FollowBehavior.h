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
		public:
			float distance;
			float maxDistance;
			std::string targetCategory;

		/////////////////////////////////////
		// Constructors
		/////////////////////////////////////
		public:
			FollowBehavior(qfcbase::Entity* parent);
			~FollowBehavior();

		/////////////////////////////////////
		// Methods
		/////////////////////////////////////
		public:
			virtual void Update(double dt);
	};
}
