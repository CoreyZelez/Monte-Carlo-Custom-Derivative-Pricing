#include "pch.h"
#include "FinanceMath.h"
#include <math.h>

double calulatePresentValue(double futureValue, int numYearlyTradingDays, int numDays, double discountRate)
{
    double totalDiscount = discountRate * numDays / numYearlyTradingDays;
    return futureValue * exp(-totalDiscount);
        ;
}
