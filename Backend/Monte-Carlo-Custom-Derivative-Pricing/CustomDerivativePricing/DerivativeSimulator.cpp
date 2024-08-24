#include "pch.h"
#include "DerivativeSimulator.h"
#include <thread>

DerivativeSimulator::DerivativeSimulator(AssetModel model)
	: model(std::move(model))
{
}

void DerivativeSimulator::runAssetSimulations(int numThreads, int numSimulations, int numDays)
{
	data.clear();
	data.resize(numThreads * numSimulations);

    std::vector<std::thread> threads;

    for(int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(&DerivativeSimulator::simulateAsset, this, i, numSimulations, numDays);
    }

    for(auto& thread : threads)
    {
        thread.join();
    }
}

void DerivativeSimulator::simulateAsset(int threadNum, int numSimulations, int numDays)
{
    for(int n = 0; n < numSimulations; ++n)
    {
        AssetModel model = this->model;

        const int simIndex = numSimulations * threadNum + n;
        model.initData();
        data[simIndex][0] = model.getData();

        for(int day = 1; day <= numDays; ++day)
        {
            model.advance();
            data[simIndex][day] = model.getData();
        }
    }
}

extern "C"
{
    CUSTOM_DERIVATIVE_PRICING_API DerivativeSimulator* DerivativeSimulator_new(AssetModel* model) { return new DerivativeSimulator(*model); }
    CUSTOM_DERIVATIVE_PRICING_API void DerivativeSimulator_delete(DerivativeSimulator* obj) { delete obj; }
    CUSTOM_DERIVATIVE_PRICING_API void DerivativeSimulator_runSimulations(DerivativeSimulator* obj, int numThreads, int numSimulations, int numDays) { obj->runAssetSimulations(numThreads, numSimulations, numDays); }
}