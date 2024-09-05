#pragma once

/// @brief Calculates the present value of some future value by continuous discounting.
/// @param futureValue The future value.
/// @param numYearlyTradingDays The number of trading days in a year.
/// @param numDays The number of trading days to discount the future value.
/// @param discountRate The continuous per annum discount rate.
double calulatePresentValue(double futureValue, int numYearlyTradingDays, int numDays, double discountRate);

