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

	virtual std::unique_ptr<AssetDerivative> clone() const override;

	virtual bool isExecutable() const override;

protected:
	virtual double calculateExecutionValue(const std::map<AssetDataClass, std::any>& data) const override;
	virtual double calculateAccumulationValue(const std::map<AssetDataClass, std::any>& data, double discountRate) const override;

private:
	OptionType type;
	OptionStyle style;
	int expiryDay;
	double strikePrice;
};


extern "C"
{
	CUSTOM_DERIVATIVE_PRICING_API VanillaOption* vanilla_option_create(int numTradingDays, VanillaOption::OptionType type, VanillaOption::OptionStyle style, int expiryDay, double strikePrice);
	CUSTOM_DERIVATIVE_PRICING_API void vanilla_option_delete(VanillaOption* option);
}
