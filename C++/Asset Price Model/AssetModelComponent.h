
/// @brief Component of asset pricing model. Influences price and volatility of asset.
class AssetModelComponent
{
public:
    /// @brief Adjusts the expected return of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param expectedReturn The current continuously compounded expected return of the asset.
    virtual void adjustExpectedReturn(int day, double& expectedReturn) = 0;

    /// @brief Adjusts the baseline annual volatility of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param price The current price of the asset.
    /// @param volatility The baseline annual volatility of the asset.
    virtual void adjustVolatility(int day, double price, double &volatility) = 0;

    /// @brief Adjusts the volatility of the asset on the current day.
    /// @param day The current day being simulated for the asset price model.
    /// @param price The current price of the asset.
    /// @param dayVolatility The daily volatility of the asset on the current day.
    virtual void adjustDayVolatility(int day, double price, double& dayVolatility) = 0;

    /// @brief Adjusts the price of the asset.
    /// @param day The current day being simulated for the asset price model.
    /// @param price The current price of the asset.
    /// @param dayVolatility The daily volatility of the asset for the current day.
    virtual void adjustPrice(int day, double& price, double dayVolatility) = 0;
};
