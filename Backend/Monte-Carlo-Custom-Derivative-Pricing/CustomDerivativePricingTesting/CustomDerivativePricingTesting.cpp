#include "AssetModel.h"
#include "DerivativeSimulator.h"
#include "FixedDividendFactor.h"
#include <iostream>

int main()
{
    AssetModel assetModel;
    assetModel.setNumTradingDays(252);
    assetModel.setPrice(50);
    assetModel.setExpectedReturn(0.1);
    assetModel.setVolatility(0.2);
    assetModel.addFactor(FixedDividendFactor(10, 30, 10));
    DerivativeSimulator simulator(assetModel);

    simulator.runAssetSimulations(8, 4000, 600);
}