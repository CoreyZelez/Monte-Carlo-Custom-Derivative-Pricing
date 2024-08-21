#pragma once
#include <any>

/// An event impacting a given asset model for a particular period of time. 
struct AssetEvent
{
	enum class Type
	{
		Dividend
	};
	
	int day;  // Day event takes place. Event may extend beyond this day.
	Type event;
	std::any other;  // Other details describing the event.
};

