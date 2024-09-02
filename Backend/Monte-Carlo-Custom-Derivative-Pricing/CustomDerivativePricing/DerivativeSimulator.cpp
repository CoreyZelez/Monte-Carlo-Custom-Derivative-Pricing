#include "pch.h"
#include "DerivativeSimulator.h"
#include <thread>
#include <iostream>

DerivativeSimulator::DerivativeSimulator(std::unique_ptr<AssetDerivative> derivative, AssetModel model)
	: derivative(std::move(derivative)), model(std::move(model))
{
}

void DerivativeSimulator::runAssetSimulations(int numThreads, int numSimulations, int numDays)
{
    assetData.clear();
    derivativePresentValues.clear();
    derivativeExecutionValues.clear();

    assetData.resize(numThreads * assetDataRecordedSimulations);
    derivativePresentValues.resize(numThreads * numSimulations);
    derivativeExecutionValues.resize(numThreads * numSimulations);

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

bool DerivativeSimulator::hasAssetData(int simIndex, int day, AssetDataClass dataClass) const
{
    return assetData[simIndex].at(day).count(dataClass) == 1;
}

std::any DerivativeSimulator::getAssetData(int simIndex, int day, AssetDataClass dataClass) const
{
    return assetData[simIndex].at(day).at(dataClass);
}

void DerivativeSimulator::simulateAsset(int threadNum, int numSimulations, int numDays)
{
    for(int n = 0; n < numSimulations; ++n)
    {
        AssetModel model = this->model;

        const int simIndex = numSimulations * threadNum + n;

        model.initData();
        recordAssetData(model, simIndex, 0);

        for(int day = 1; day <= numDays; ++day)
        {
            model.advance();
            recordAssetData(model, simIndex, day);

            assetData[simIndex][day] = model.getData();
        }
    }
}
void DerivativeSimulator::recordAssetData(const AssetModel& model, int simIndex, int day)
{
    if(simIndex < assetDataRecordedSimulations)
    {
        assetData[simIndex][day] = model.getData();
    }
}


extern "C"
{
    CUSTOM_DERIVATIVE_PRICING_API DerivativeSimulator* DerivativeSimulator_new(AssetDerivative* derivative, AssetModel* model) { return new DerivativeSimulator(std::unique_ptr<AssetDerivative>(derivative), *model); }
    CUSTOM_DERIVATIVE_PRICING_API void DerivativeSimulator_delete(DerivativeSimulator* obj) { delete obj; }
    CUSTOM_DERIVATIVE_PRICING_API void DerivativeSimulator_runAssetSimulations(DerivativeSimulator* obj, int numThreads, int numSimulations, int numDays) { obj->runAssetSimulations(numThreads, numSimulations, numDays); }
    CUSTOM_DERIVATIVE_PRICING_API bool DerivativeSimulator_hasAssetData(DerivativeSimulator* obj, int simIndex, int day, int assetDataClass) { return obj->hasAssetData(simIndex, day, static_cast<AssetDataClass>(assetDataClass)); }
    CUSTOM_DERIVATIVE_PRICING_API int DerivativeSimulator_getIntAssetData(DerivativeSimulator* obj, int simIndex, int day, int assetDataClass) { return std::any_cast<int>(obj->getAssetData(simIndex, day, static_cast<AssetDataClass>(assetDataClass))); }
}