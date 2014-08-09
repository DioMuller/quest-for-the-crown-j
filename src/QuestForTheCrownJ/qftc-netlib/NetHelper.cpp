#include "NetHelper.h"
#include "Log.h"

std::string NetHelper::DecodeAnimation(int info)
{
	switch (info)
	{
		case 0: return "stopped_left";
		case 1: return "stopped_up";
		case 2: return "stopped_right";
		case 3: return "stopped_down";
		case 10: return "walking_left";
		case 11: return "walking_up";
		case 12: return "walking_right";
		case 13: return "walking_down";
	}
	return "";
}

int NetHelper::EncodeAnimation(std::string animation)
{
	if (animation == "stopped_left")
		return 0;
	if (animation == "stopped_up")
		return 1;
	if (animation == "stopped_right")
		return 2;
	if (animation == "stopped_down")
		return 3;

	if (animation == "walking_left")
		return 10;
	if (animation == "walking_up")
		return 11;
	if (animation == "walking_right")
		return 12;
	if (animation == "walking_down")
		return 13;

	return -1;
}

EntityCategory NetHelper::EncodeCategory(std::string category)
{
	if (category == "GoodGuy")
		return EntityCategory::GOOD_GUY;
	if (category == "Enemy")
		return EntityCategory::ENEMY;

	qfcbase::Log::Error((std::string)"Entity category not supported: " + category);
	return EntityCategory::CATEGORY_UNKNOWN;
}

std::string NetHelper::DecodeCategory(EntityCategory category)
{
	switch (category)
	{
	case GOOD_GUY:
		return "GoodGuy";
	case ENEMY:
		return "Enemy";
	}

	qfcbase::Log::Error((std::string)"Entity category not supported: " + std::to_string(category));
	return "";
}
