#pragma once
#include "AssetModelFactor.h"

/// Dividend with fixed details.
class FixedDividendFactor : public AssetModelFactor
{
public:
    FixedDividendFactor(int announcementDay, int paymentDay, double dividendAmount);

    virtual std::unique_ptr<AssetModelFactor> clone() const;

    virtual void addScheduledEvents(int day, std::vector<AssetEvent>& scheduledEvents);
    virtual void adjustExpectedReturn(int day, double& expectedReturn);
    virtual void adjustVolatility(int day, double& volatility);
    virtual void adjustDayVolatility(int day, double& dayVolatility);
    virtual void adjustPrice(int day, double& price);
private:
    int announcementDay;
    int paymentDay;
    double dividendAmount;
};

