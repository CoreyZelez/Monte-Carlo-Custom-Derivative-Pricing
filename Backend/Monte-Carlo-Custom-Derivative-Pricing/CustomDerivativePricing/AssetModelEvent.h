#pragma once
#include <any>

/// An event impacting a given asset model for a particular period of time. 
struct AssetModelEvent
{
	enum class EventType
	{
		Dividend
	};
	
	int day;  // Day event takes place.
	EventType eventType;
	std::any details;
};

