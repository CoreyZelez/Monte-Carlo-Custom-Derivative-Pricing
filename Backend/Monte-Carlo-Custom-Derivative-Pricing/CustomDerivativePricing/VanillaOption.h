#pragma once
#include "AssetDerivative.h"

class VanillaOption : public AssetDerivative
{
public:
	enum class OptionType
	{
		Put,
		Call
	};

	enum class OptionStyle
	{
		American,
		European
	};

	VanillaOption(OptionType type, OptionStyle style, int expiryDay, double strikePrice);

	virtual void update(int day, double price) override;
	virtual bool isActionable() const override;
	virtual double calculatePayoff() const override;
	virtual double calculatePresentValue(int numTradingDays, double discountRate) const override;

private:
	OptionType type;
	OptionStyle style;
	int expiryDay;
	double strikePrice;
	int currentDay = -1;
	double currentAssetPrice = -1;

};

