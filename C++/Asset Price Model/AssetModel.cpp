#include "AssetModel.h"
#include <math.h>
#include <random>

AssetModel::AssetModel()
    : gen(std::random_device{}()), normalDist(1, 0) {}

double AssetModel::advance()
{
    advanceExpectedReturn();
    advanceVolatility();
    advanceDayVolatility();
    advancePrice();
}

void AssetModel::advanceExpectedReturn()
{
    for(auto& component : components)
    {
        component.get()->adjustExpectedReturn(day, expectedReturn);
    }
}

void AssetModel::advanceVolatility()
{
    for(auto& component : components)
    {
        component.get()->adjustVolatility(day, price, volatility);
    }
}

void AssetModel::advanceDayVolatility()
{
    dayVolatility = volatility / sqrt(numTradingDays);

    for(auto& component : components)
    {
        component.get()->adjustDayVolatility(day, price, dayVolatility);
    }
}

void AssetModel::advancePrice()
{
    double dailyExpectedReturn = expectedReturn / numTradingDays;  // Continuously compounded.
    price = (price * exp(dailyExpectedReturn)) + (price * dayVolatility * normalDist(gen));

    for(auto& component : components)
    {
        component.get()->adjustPrice(day, price, dayVolatility);
    }
}
