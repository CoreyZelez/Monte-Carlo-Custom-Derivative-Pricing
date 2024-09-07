#include "pch.h"
#include "AssetDerivative.h"
#include "FinanceMath.h"

AssetDerivative::AssetDerivative(int numTradingDays)
    : numTradingDays(numTradingDays)
{
}

void AssetDerivative::update(int day, double discountRate, const std::map<AssetDataClass, std::any>& assetData)
{
    this->day = day;
    executionValue = calculateExecutionValue(assetData);
    accumulationValue = calculateAccumulationValue(assetData, discountRate);
    totalValue = executionValue + calulatePresentValue(executionValue, getNumTradingDays(), day, discountRate);
    
    data.clear();
    data[DerivativeDataClass::Execution] = executionValue;
    data[DerivativeDataClass::Accumulation] = accumulationValue;
    data[DerivativeDataClass::Total] = totalValue;
}

double AssetDerivative::getTotalValue() const
{
    return totalValue;
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
