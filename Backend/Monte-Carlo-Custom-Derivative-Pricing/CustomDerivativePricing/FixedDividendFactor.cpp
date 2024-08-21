#include "pch.h"
#include "FixedDividendFactor.h"
#include "AssetEvent.h"

void FixedDividendFactor::addScheduledEvents(int day, std::vector<AssetEvent>& scheduledEvents)
{
	if(day == announcementDay)
	{
		AssetEvent event = { paymentDay, AssetEvent::Type::Dividend, dividendAmount };
		scheduledEvents.push_back(event);
	}
}
