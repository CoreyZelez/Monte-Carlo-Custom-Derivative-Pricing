#pragma once
#include "AssetModelFactor.h"
#include "AssetDataClass.h"
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
    AssetModel(int numTradingDays, double price, double expectedReturn, double volatility, std::vector<std::unique_ptr<AssetModelFactor>> &factors);
    AssetModel(const AssetModel& other);
    AssetModel& operator=(const AssetModel& other) = delete;

    void addFactor(const AssetModelFactor& factor);

    /// @brief Sets data for day 0.
    void initData();

    /// @brief Advances the asset model by a day.
    void advance();

    int getDay() const;
    double getPrice() const;
    double getExpectedReturn() const;
    double getVolatility() const;
    double getDayVolatility() const;
    const std::map<AssetDataClass, std::any>& getData() const;

private:
    int numTradingDays;
    int day = 0;  // Current day of model.
    double price;  // Current asset price.
    double expectedReturn;  // Continuously compounded annual expected return of the asset. 
    double volatility;  // Annualised volatility of the asset.
    double dayVolatility = -1;  // Day volatility of the most recent day simulated.
    std::vector<std::unique_ptr<AssetModelFactor>> factors;  // Custom factors influencing price, expected return, and volatility.
    std::map<AssetDataClass, std::any> data;
    std::mt19937 gen;
    std::normal_distribution<double> normalDist;

    void advanceExpectedReturn();
    void advanceVolatility();
    void advanceDayVolatility();
    void advancePrice();
    void advanceData();
};

extern "C"
{
    CUSTOM_DERIVATIVE_PRICING_API AssetModel* asset_model_create(int numTradingDays, double price, double expectedReturn, double volatility, std::vector<std::unique_ptr<AssetModelFactor>>* factors);
    CUSTOM_DERIVATIVE_PRICING_API void asset_model_delete(AssetModel* model);
}