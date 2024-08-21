#pragma once
#include "AssetModelFactor.h"
#include "AssetEvent.h"
#include <vector>
#include <memory>
#include <random>

class AssetModel
{
public:
    AssetModel();
    AssetModel(const AssetModel& other);

    /// @brief Advances the asset model by a day.
    void advance();

    void setPrice(double price);
    void setExpectedReturn(double expectedReturn);
    void setVolatility(double volatility);

    double getPrice() const;
    double getExpectedReturn() const;
    double getVolatility() const;
    double getDayVolatility() const;

private:
    int day = 0;  // Current day of model.
    double price = 0;  // Current asset price.
    double expectedReturn = 0;  // Continuously compounded annual expected return of the asset. 
    double volatility = 0;  // Annualised volatility of the asset.
    double dayVolatility = 0;  // Day volatility of the most recent day simulated.
    const int numTradingDays = 252;

    std::vector<std::unique_ptr<AssetModelFactor>> factors;  // Custom factors influencing price, expected return, and volatility.
    std::vector<AssetEvent> scheduledEvents;  // Upcoming events known to investor.

    std::mt19937 gen;
    std::normal_distribution<double> normalDist;

    void advanceExpectedReturn();
    void advanceVolatility();
    void advanceDayVolatility();
    void advancePrice();
    void advanceScheduledEvents();
};

