#include "pch.h"
#include "AssetModel.h"
#include <math.h>
#include <random>
#include <assert.h>

AssetModel::AssetModel(int numTradingDays, double price, double expectedReturn, double volatility, std::vector<std::unique_ptr<AssetModelFactor>>& factors)
    : numTradingDays(numTradingDays), price(price), expectedReturn(expectedReturn), volatility(volatility), factors(std::move(factors)), 
    gen(std::random_device{}()), normalDist(0, 1) 
{}


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
    factors.reserve(other.factors.size());
    for(const auto& factor : other.factors) 
    {
        if(factor) 
        {
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

int AssetModel::getDay() const
{
    return day;
}

double AssetModel::getPrice() const
{
    return price;
}

double AssetModel::getExpectedReturn() const
{
    return expectedReturn;
}

double AssetModel::getVolatility() const
{
    return volatility;
}

double AssetModel::getDayVolatility() const
{
    return dayVolatility;
}

const std::map<AssetDataClass, std::any>& AssetModel::getData() const
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

extern "C"
{
    AssetModel* asset_model_create(int numTradingDays, double price, double expectedReturn, double volatility, std::vector<std::unique_ptr<AssetModelFactor>>* factors)
    {
        return new AssetModel(numTradingDays, price, expectedReturn, volatility, *factors);
    }

    void asset_model_delete(AssetModel* model)
    {
        delete model;
    }
}