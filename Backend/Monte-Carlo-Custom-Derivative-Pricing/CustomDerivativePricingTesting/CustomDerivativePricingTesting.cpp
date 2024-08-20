#include "AssetModel.h"
#include "AssetModel.cpp"
#include "AssetPriceSimulator.h"
#include "AssetPriceSimulator.cpp"
#include <iostream>

int main()
{
    AssetModel assetModel;
    assetModel.setPrice(50);
    assetModel.setExpectedReturn(0.1);
    assetModel.setVolatility(0.2);
    AssetPriceSimulator simulator(assetModel);

    simulator.runSimulations(8, 100000, 600);
    for(double price : simulator.getPrices()[0])
    {
        int d = 0;
        std::cout << price << " " << d++ << std::endl;
    }
}