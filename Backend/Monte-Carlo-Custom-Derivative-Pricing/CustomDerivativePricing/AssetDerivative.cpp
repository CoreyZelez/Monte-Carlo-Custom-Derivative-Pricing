#include "pch.h"
#include "AssetDerivative.h"

AssetDerivative::AssetDerivative(int numTradingDays)
    : numTradingDays(numTradingDays)
{
}

void AssetDerivative::update(int day, const std::map<std::string, std::any>& data, double discountRate)
{
    this->day = day;
    executionValue = calculateExecutionValue(data);
    accumulationValue = calculateAccumulationValue(data, discountRate);
}

double AssetDerivative::getExecutionValue() const
{
    return executionValue;
}

double AssetDerivative::getAccumulationValue() const
{
    return accumulationValue;
}

int AssetDerivative::getNumTradingDays() const
{
    return numTradingDays;
}

int AssetDerivative::getDay() const
{
    return day;
}
