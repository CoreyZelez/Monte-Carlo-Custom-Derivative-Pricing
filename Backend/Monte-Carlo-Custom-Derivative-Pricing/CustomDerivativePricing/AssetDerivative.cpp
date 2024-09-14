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
    totalValue = accumulationValue + calulatePresentValue(executionValue, getNumTradingDays(), day, discountRate);

    // IN FUTURE WE WILL HAVE TO RECALCULATE THE TRUE DISCOUNT EVERY UPDATE SINCE IF DISCOUNT RATE CHANGES OVER TIME WE MUST TAKE INTO
    // ACCOUNT EVERY VALUE THROUGHOUT ITS LIFE AS    EXP(-R1) * EXP(-R2) * ... * EXP(-Rn) where Ri is the cont daily disc on day i.
    
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

const std::map<DerivativeDataClass, double>& AssetDerivative::getData() const
{
    return data;
}
