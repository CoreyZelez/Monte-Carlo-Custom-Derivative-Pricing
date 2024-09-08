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

void FixedDividendFactor::addData(int day, std::map<AssetDataClass, std::any>& data)
{
	if(day == announcementDay)
	{
		data[AssetDataClass::DividendAnnouncement] = std::make_pair(paymentDay, dividendAmount);
	}

	if(day == paymentDay)
	{
		data[AssetDataClass::Dividend] = dividendAmount;
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

extern "C" 
{
	FixedDividendFactor* fixed_dividend_factor_create(int announcementDay, int paymentDay, double dividendAmount) 
	{
		return new FixedDividendFactor(announcementDay, paymentDay, dividendAmount);
	}

	void fixed_dividend_factor_delete(FixedDividendFactor* factor) 
	{
		delete factor;
	}
}
