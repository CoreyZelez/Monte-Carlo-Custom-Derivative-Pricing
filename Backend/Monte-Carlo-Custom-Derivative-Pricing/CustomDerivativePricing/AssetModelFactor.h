#pragma once
#include <memory>
#include <vector>

class AssetEvent;

/// @brief Factor of asset model affecting price, volatility, or expected return.
class AssetModelFactor
{
public:
    /// @brief Clones this object into a unique pointer.
    /// @return Unique pointer to clone of this object.
    virtual std::unique_ptr<AssetModelFactor> clone() const = 0;

    /// @brief Adjusts the expected return of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param scheduledEvents The current upcoming scheduled events for the price.
    virtual void addScheduledEvents(int day, std::vector<AssetEvent> &scheduledEvents) = 0;

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
