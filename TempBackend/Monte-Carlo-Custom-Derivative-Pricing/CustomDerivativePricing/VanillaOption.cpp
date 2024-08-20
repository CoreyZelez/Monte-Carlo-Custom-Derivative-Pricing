#include "pch.h"
#include "VanillaOption.h"
#include <assert.h>
#include <algorithm>     

VanillaOption::VanillaOption(OptionType type, OptionStyle style, int expiryDay, double strikePrice)
	: type(type), style(style), expiryDay(expiryDay), strikePrice(strikePrice)
{
}

void VanillaOption::update(int day, double price)
{
	assert(currentDay == day - 1);
	currentDay = day;
	currentAssetPrice = price;
}

bool VanillaOption::isActionable() const
{
	return style == OptionStyle::American || currentDay == expiryDay;
}

double VanillaOption::calculatePayoff() const
{
	if(currentDay > expiryDay)
	{
		return 0;
	}

	if(type == OptionType::Call)
	{
		return std::max<double>(currentAssetPrice - strikePrice, 0);
	}
	else
	{
		return std::max<double>(strikePrice - currentAssetPrice, 0);
	}
}

double VanillaOption::calculatePresentValue(int numTradingDays, double discountRate) const
{
	if(currentDay > expiryDay)
	{
		return 0;
	}

	const double discount = discountRate * (currentDay / numTradingDays);  // Continuously compounded.
	return calculatePayoff() * exp(-discountRate);
}
