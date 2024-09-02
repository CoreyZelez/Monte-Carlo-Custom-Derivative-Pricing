#pragma once
#include "AssetDataClass.h"
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <any>

#ifdef CUSTOM_DERIVATIVE_PRICING_API_EXPORTS
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllexport)
#else
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllimport)
#endif

struct AssetEvent;

/// @brief Factor of asset model affecting price, volatility, or expected return.
class CUSTOM_DERIVATIVE_PRICING_API AssetModelFactor
{
public:
    /// @brief Clones this object into a unique pointer.
    /// @return Unique pointer to clone of this object.
    virtual std::unique_ptr<AssetModelFactor> clone() const = 0;

    /// @brief Adds data relating to factor on the current day.
    /// @param day Day that data is being added for.
    /// @param data Data of asset relating to days evaluated for.
    virtual void addData(int day, std::map<AssetDataClass, std::any>& data) = 0;

    /// @brief Adjusts the expected return of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param expectedReturn The current continuously compounded expected return of the asset.
    virtual void adjustExpectedReturn(int day, double& expectedReturn) = 0;

    /// @brief Adjusts the baseline annual volatility of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param volatility The baseline annual volatility of the asset.
    virtual void adjustVolatility(int day, double& volatility) = 0;

    /// @brief Adjusts the volatility of the asset on the current day.
    /// @param day The current day being simulated for the asset price model.
    /// @param dayVolatility The daily volatility of the asset on the current day.
    virtual void adjustDayVolatility(int day, double& dayVolatility) = 0;

    /// @brief Adjusts the price of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param price The current price of the asset.
    virtual void adjustPrice(int day, double& price) = 0;
};
