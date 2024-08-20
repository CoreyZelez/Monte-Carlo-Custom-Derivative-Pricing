#include "pch.h"
#include "AssetPriceSimulator.h"
#include <thread>

AssetPriceSimulator::AssetPriceSimulator(AssetModel model)
	: model(std::move(model))
{
}

void AssetPriceSimulator::runSimulations(int numThreads, int numSimulations, int numDays)
{
	prices.clear();
	prices.resize(numThreads * numSimulations);

	for(auto& priceVec : prices)
	{
		priceVec.resize(numDays);
	}

    std::vector<std::thread> threads;

    for(int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(&AssetPriceSimulator::simulatePrices, this, i, numSimulations, numDays);
    }

    for(auto& thread : threads)
    {
        thread.join();
    }
}

const std::vector<std::vector<double>>& AssetPriceSimulator::getPrices() const
{
    return prices;
}

void AssetPriceSimulator::simulatePrices(int threadNum, int numSimulations, int numDays)
{
    for(int n = 0; n < numSimulations; ++n)
    {
        AssetModel model = this->model;

        const int simIndex = numSimulations * threadNum + n;
        prices[simIndex][0] = model.getPrice();

        for(int day = 1; day <= numDays; ++day)
        {
            model.advance();
            prices[simIndex][day] = model.getPrice();
        }
    }
}
