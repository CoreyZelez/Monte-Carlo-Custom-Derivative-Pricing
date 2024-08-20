#pragma once

class AssetDerivative
{
public:
	/// @brief The value of the derivative is exercised immediately. 
	/// @param day The current day for evaluating the derivative.
	/// @param price The price of the asset on the specified day.
	virtual void update(int day, double price) = 0;

	/// @brief Signals whether some particular special action, such as exercising or converting, can presently be taken on the derivative. 
	/// @return Whether an action can be taken.
	virtual bool isActionable() const = 0;

	/// @brief The value of the derivative if exercised immediately. 
	/// @return The payoff of the derivative.
	virtual double calculatePayoff() const = 0;

	/// @brief The present value of the derivative.
	/// @param discountRate The continuous rate used to determine the present value of the asset.
	/// @return The present value of the derivative discounted to the day of inception.
	virtual double calculatePresentValue(int numTradingDays, double discountRate) const  = 0;
};

