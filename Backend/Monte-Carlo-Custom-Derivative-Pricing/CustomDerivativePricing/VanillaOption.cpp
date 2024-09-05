#include "pch.h"
#include "VanillaOption.h"
#include <assert.h>
#include <algorithm>     

VanillaOption::VanillaOption(int numTradingDays, OptionType type, OptionStyle style, int expiryDay, double strikePrice)
	: AssetDerivative(numTradingDays), type(type), style(style), expiryDay(expiryDay), strikePrice(strikePrice) {}

std::unique_ptr<AssetDerivative> VanillaOption::clone() const
{
	return std::make_unique<VanillaOption>(getNumTradingDays(), type, style, expiryDay, strikePrice);
}

bool VanillaOption::isExecutable() const
{
	return style == OptionStyle::American || getDay() == expiryDay;
}

double VanillaOption::calculateExecutionValue(const std::map<std::string, std::any>& data) const
{
	double currentAssetPrice = *data.at("Asset_Price")._Cast<double>();             /////////////////////// OUTDATED!!!!!!!!!!

	if(type == OptionType::Call)
	{
		return std::max<double>(currentAssetPrice - strikePrice, 0);
	}
	else
	{
		return std::max<double>(strikePrice - currentAssetPrice, 0);
	}
}

double VanillaOption::calculateAccumulationValue(const std::map<std::string, std::any>& data, double discountRate) const
{
	return 0;
}