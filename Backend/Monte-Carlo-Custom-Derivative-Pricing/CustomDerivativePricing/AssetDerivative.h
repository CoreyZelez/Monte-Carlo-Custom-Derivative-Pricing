#pragma once
#include "DerivativeDataClass.h"
#include "AssetDataClass.h"
#include <map>
#include <string>
#include <any>
#include <memory>

#ifdef CUSTOM_DERIVATIVE_PRICING_API_EXPORTS
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllexport)
#else
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllimport)
#endif

class CUSTOM_DERIVATIVE_PRICING_API AssetDerivative
{
public:
	AssetDerivative(int numTradingDays);

	virtual std::unique_ptr<AssetDerivative> clone() const = 0;

	/// @brief Updates the derivative 
	/// @param day The current day for evaluating the derivative.
	/// @param discountRate Continuous rate at which to discount cash flows occurring on current day.
	/// @param assetData Data of asset relating to days evaluated for.
	virtual void update(int day, double discountRate, const std::map<AssetDataClass, std::any> &assetData);

	/// @brief Signals whether the derivative can be executed on the most recent evaluated day.
	/// @return Whether the derivative can be executed.
	virtual bool isExecutable() const = 0;

	double getTotalValue() const;
	double getExecutionValue() const;
	double getAccumulationValue() const;
	int getNumTradingDays() const;
	int getDay() const;
	const std::map<DerivativeDataClass, double>& getData() const;

protected:
	/// @brief Determines the execution value of the derivative. 
	/// @param data Data of asset relating to days evaluated for.
	/// @return The new execution value of the derivative.
	virtual double calculateExecutionValue(const std::map<AssetDataClass, std::any>& data) const = 0;

	/// @brief Determines the discounted value of the derivative from accumulated and current cashflows received if executed. 
	/// @brief Discounted value of non-executable derivatives will be calculated assuming executability if applicable.
	/// @param data Data of asset relating to days evaluated for.
	/// @return The discounted value of the derivative on the current day, otherwise -1 if not applicable.
	virtual double calculateAccumulationValue(const std::map<AssetDataClass, std::any>& data, double discountRate) const = 0;

private:
	const int numTradingDays;
	int day = -1;
	double totalValue = 0;  // Present value of all accumulated cashflows and cashflow from execution.
	double accumulationValue = 0;  // Present value of all accumulated cashflows.
	double executionValue = 0;  // Cashflow received from execution on current day. Not discounted to present day.
	std::map<DerivativeDataClass, double> data;
};

