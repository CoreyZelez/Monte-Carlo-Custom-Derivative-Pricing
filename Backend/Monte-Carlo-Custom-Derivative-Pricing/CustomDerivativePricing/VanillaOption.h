#pragma once
#include "AssetDerivative.h"
#include <map>
#include <string>
#include <any>

#ifdef CUSTOM_DERIVATIVE_PRICING_API_EXPORTS
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllexport)
#else
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllimport)
#endif

class CUSTOM_DERIVATIVE_PRICING_API VanillaOption : public AssetDerivative
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

	VanillaOption(int numTradingDays, OptionType type, OptionStyle style, int expiryDay, double strikePrice);

	virtual bool isExecutable() const override;

protected:
	virtual double calculateExecutionValue(const std::map<std::string, std::any>& data) const override;
	virtual double calculateAccumulationValue(const std::map<std::string, std::any>& data, double discountRate) const override;

private:
	OptionType type;
	OptionStyle style;
	int expiryDay;
	double strikePrice;
};

