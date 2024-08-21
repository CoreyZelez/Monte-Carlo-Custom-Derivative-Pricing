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

void AssetModel::advance()
{
    ++day;
    advanceScheduledEvents();
    advanceExpectedReturn();
    advanceVolatility();
    advanceDayVolatility();
    advancePrice();
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

void AssetModel::advanceScheduledEvents()
{
    for(auto& factor : factors)
    {
        factor.get()->addScheduledEvents(day, scheduledEvents);
    }

    for(auto iter = scheduledEvents.begin(); iter != scheduledEvents.end();)
    {
        if(iter->day < day)
        {
            iter = scheduledEvents.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}
