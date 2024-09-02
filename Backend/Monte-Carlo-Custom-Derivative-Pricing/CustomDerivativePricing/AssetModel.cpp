#include "pch.h"
#include "AssetModel.h"
#include <math.h>
#include <random>
#include <assert.h>

AssetModel::AssetModel()
    : gen(std::random_device{}()), normalDist(0, 1) {}


AssetModel::AssetModel(const AssetModel& other) 
    :  
    price(other.price),
    expectedReturn(other.expectedReturn),
    volatility(other.volatility),
    dayVolatility(other.dayVolatility),
    day(other.day),
    numTradingDays(other.numTradingDays),
    gen(other.gen),
    normalDist(other.normalDist)
{
    // Deep copy of the components vector using the clone method
    factors.reserve(other.factors.size());
    for(const auto& factor : other.factors) {
        if(factor) {
            factors.push_back(factor->clone());
        }
    }
}

void AssetModel::addFactor(const AssetModelFactor& factor)
{
    factors.push_back(factor.clone());
}

void AssetModel::initData()
{
    advanceData();
}

void AssetModel::advance()
{
    ++day;
    advanceExpectedReturn();
    advanceVolatility();
    advanceDayVolatility();
    advancePrice();
    advanceData();
}

void AssetModel::setNumTradingDays(int numTradingDays)
{
    assert(day == 0);

    this->numTradingDays = numTradingDays;
}

void AssetModel::setPrice(double price)
{
    assert(day == 0);

    this->price = price;
}

void AssetModel::setExpectedReturn(double expectedReturn)
{
    assert(day == 0);

    this->expectedReturn = expectedReturn;
}

void AssetModel::setVolatility(double volatility)
{
    assert(day == 0);

    this->volatility = volatility;
    dayVolatility = volatility / sqrt(numTradingDays);
}

std::map<AssetDataClass, std::any> AssetModel::getData() const
{
    return data;
}

void AssetModel::advanceData() 
{
    data.clear();
    data[AssetDataClass::Price] = price;
    data[AssetDataClass::Volatility] = volatility;
    data[AssetDataClass::DayVolatility] = dayVolatility;

    for(const auto &factor : factors)
    {
        factor.get()->addData(day, data);
    }
}

void AssetModel::advanceExpectedReturn()
{
    for(auto& factor : factors)
    {
        factor.get()->adjustExpectedReturn(day, expectedReturn);
    }
}

void AssetModel::advanceVolatility()
{
    for(auto& factor : factors)
    {
        factor.get()->adjustVolatility(day, price);
    }
}

void AssetModel::advanceDayVolatility()
{
    dayVolatility = volatility / sqrt(numTradingDays); 

    for(auto& factor : factors)
    {
        factor.get()->adjustDayVolatility(day, price);
    }
}

void AssetModel::advancePrice()
{
    // Price change due to model based on Itô process.
    const double dailyExpectedReturn = expectedReturn / numTradingDays;  // Continuously compounded.
    price = (price * exp(dailyExpectedReturn)) + (price * dayVolatility * normalDist(gen));

    for(auto& factor : factors)
    {
        factor.get()->adjustPrice(day, price);
    }
}
