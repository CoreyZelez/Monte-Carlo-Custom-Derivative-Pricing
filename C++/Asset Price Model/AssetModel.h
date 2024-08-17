#include "AssetModelComponent.h"
#include <vector>
#include <memory>
#include <random>

class AssetModel
{
public:
    AssetModel();

    /// @brief Advances the asset model by a day.
    /// @return The price of the asset following the simulation of the day.
    double advance();

private:
    double price = 0;  // Current asset price.
    double expectedReturn = 0;  // Continuously compounded annual expected return of the asset. 
    double volatility = 0;  // Baseline annualised volatility of the asset.
    double dayVolatility = 0;  // Day volatility of the most recent day simulated.
    int day = 0;  // Current day of model.
    const int numTradingDays = 252;

    std::vector<std::unique_ptr<AssetModelComponent>> components;  // Custom components affecting asset model.

    const std::mt19937 gen;
    std::normal_distribution<double> normalDist;

    void advanceExpectedReturn();
    void advanceVolatility();
    void advanceDayVolatility();
    void advancePrice();
};