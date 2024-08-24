#pragma once
#include "AssetModelFactor.h"
#include <vector>
#include <any>
#include <map>
#include <memory>
#include <random>

#ifdef CUSTOM_DERIVATIVE_PRICING_API_EXPORTS
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllexport)
#else
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllimport)
#endif

class CUSTOM_DERIVATIVE_PRICING_API AssetModel
{
public:
    AssetModel();
    AssetModel(const AssetModel& other);

    void addFactor(const AssetModelFactor& factor);

    /// @brief Sets data for day 0.
    void initData();

    /// @brief Advances the asset model by a day.
    void advance();

    void setPrice(double price);
    void setExpectedReturn(double expectedReturn);
    void setVolatility(double volatility);

    std::map<std::string, std::any> getData() const;

private:
    int day = 0;  // Current day of model.
    double price = 0;  // Current asset price.
    double expectedReturn = 0;  // Continuously compounded annual expected return of the asset. 
    double volatility = 0;  // Annualised volatility of the asset.
    double dayVolatility = 0;  // Day volatility of the most recent day simulated.
    const int numTradingDays = 252;

    std::vector<std::unique_ptr<AssetModelFactor>> factors;  // Custom factors influencing price, expected return, and volatility.

    std::map<std::string, std::any> data;

    std::mt19937 gen;
    std::normal_distribution<double> normalDist;

    void advanceExpectedReturn();
    void advanceVolatility();
    void advanceDayVolatility();
    void advancePrice();
    void advanceData();
};

