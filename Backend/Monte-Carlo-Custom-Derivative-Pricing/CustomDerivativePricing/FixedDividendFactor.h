#pragma once
#include "AssetModelFactor.h"

#ifdef CUSTOM_DERIVATIVE_PRICING_API_EXPORTS
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllexport)
#else
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllimport)
#endif

/// Dividend with fixed details.
class CUSTOM_DERIVATIVE_PRICING_API FixedDividendFactor : public AssetModelFactor
{
public:
    FixedDividendFactor(int announcementDay, int paymentDay, double dividendAmount);

    virtual std::unique_ptr<AssetModelFactor> clone() const override;

    virtual void addData(int day, std::map<AssetDataClass, std::any>& data) override;
    virtual void adjustExpectedReturn(int day, double& expectedReturn) override;
    virtual void adjustVolatility(int day, double& volatility) override;
    virtual void adjustDayVolatility(int day, double& dayVolatility) override;
    virtual void adjustPrice(int day, double& price) override;
private:
    int announcementDay;
    int paymentDay;
    double dividendAmount;
};

