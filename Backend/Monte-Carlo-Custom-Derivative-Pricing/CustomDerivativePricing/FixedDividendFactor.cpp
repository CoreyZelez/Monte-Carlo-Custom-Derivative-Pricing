#include "pch.h"
#include "FixedDividendFactor.h"

FixedDividendFactor::FixedDividendFactor(int announcementDay, int paymentDay, double dividendAmount)
	: announcementDay(announcementDay), paymentDay(paymentDay), dividendAmount(dividendAmount)
{
}

std::unique_ptr<AssetModelFactor> FixedDividendFactor::clone() const
{
	return std::make_unique<FixedDividendFactor>(announcementDay, paymentDay, dividendAmount);
}

void FixedDividendFactor::addData(int day, std::map<std::string, std::any>& data) 
{
	if(day == announcementDay)
	{
		data["Dividend_Announcement"] = std::make_pair(paymentDay, dividendAmount);
	}

	if(day == paymentDay)
	{
		data["Dividend"] = dividendAmount;
	}
}

void FixedDividendFactor::adjustExpectedReturn(int day, double& expectedReturn)
{
}

void FixedDividendFactor::adjustVolatility(int day, double& volatility)
{
}

void FixedDividendFactor::adjustDayVolatility(int day, double& dayVolatility)
{
}

void FixedDividendFactor::adjustPrice(int day, double& price)
{
	if(day == paymentDay)
	{
		price -= dividendAmount;
	}
}
