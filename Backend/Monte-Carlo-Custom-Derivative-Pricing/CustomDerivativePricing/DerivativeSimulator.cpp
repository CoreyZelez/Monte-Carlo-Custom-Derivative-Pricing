#include "pch.h"
#include "DerivativeSimulator.h"
#include <thread>
#include <iostream>

DerivativeSimulator::DerivativeSimulator(AssetModel model)
	: model(std::move(model))
{
}

void DerivativeSimulator::runSimulations(int numThreads, int numSimulations, int numDays)
{
    assetData.clear();
    derivativeData.clear();

    assetData.resize(numThreads * numSimulations);
    derivativeData.resize(numThreads * numSimulations);

    std::vector<std::thread> threads;

    for(int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(&DerivativeSimulator::simulate, this, i, numSimulations, numDays);
    }

    for(auto& thread : threads)
    {
        thread.join();
    }
}

void DerivativeSimulator::addDerivative(std::unique_ptr<AssetDerivative> derivative)
{
    derivatives.push_back(std::move(derivative));
}

bool DerivativeSimulator::hasAssetData(int simIndex, int day, AssetDataClass dataClass) const
{
    return assetData[simIndex].at(day).count(dataClass) == 1;
}

std::any DerivativeSimulator::getAssetData(int simIndex, int day, AssetDataClass dataClass) const
{
    return assetData[simIndex].at(day).at(dataClass);
}

void DerivativeSimulator::simulate(int threadNum, int numSimulations, int numDays)
{
    for(int n = 0; n < numSimulations; ++n)
    {
        AssetModel model = this->model;
        std::vector<std::unique_ptr<AssetDerivative>> derivatives;
    
        for(auto& derivative : this->derivatives)
        {
            derivatives.push_back(derivative.get()->clone());
        }
    
        const int simIndex = numSimulations * threadNum + n;
    
        model.initData();
        assetData[simIndex][0] = model.getData();
    
        for(int day = 1; day <= numDays; ++day)
        {
            model.advance();
            assetData[simIndex][day] = model.getData();
        }
    }
}

extern "C"
{
    CUSTOM_DERIVATIVE_PRICING_API DerivativeSimulator* DerivativeSimulator_new(AssetModel* model, AssetDerivative* derivative) { return new DerivativeSimulator(*model); }
    CUSTOM_DERIVATIVE_PRICING_API void DerivativeSimulator_delete(DerivativeSimulator* obj) { delete obj; }
    CUSTOM_DERIVATIVE_PRICING_API void DerivativeSimulator_runSimulations(DerivativeSimulator* obj, int numThreads, int numSimulations, int numDays) { obj->runSimulations(numThreads, numSimulations, numDays); }
    CUSTOM_DERIVATIVE_PRICING_API bool DerivativeSimulator_hasAssetData(DerivativeSimulator* obj, int simIndex, int day, int assetDataClass) { return obj->hasAssetData(simIndex, day, static_cast<AssetDataClass>(assetDataClass)); }
    CUSTOM_DERIVATIVE_PRICING_API int DerivativeSimulator_getIntAssetData(DerivativeSimulator* obj, int simIndex, int day, int assetDataClass) { return std::any_cast<int>(obj->getAssetData(simIndex, day, static_cast<AssetDataClass>(assetDataClass))); }
}