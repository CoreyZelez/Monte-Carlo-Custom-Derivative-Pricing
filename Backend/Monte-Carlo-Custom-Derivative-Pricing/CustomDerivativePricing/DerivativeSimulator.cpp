#include "pch.h"
#include "DerivativeSimulator.h"
#include <thread>
#include <iostream>

DerivativeSimulator::DerivativeSimulator(AssetModel model, std::vector<std::unique_ptr<AssetDerivative>>& derivatives)
	: model(std::move(model)), derivatives(std::move(derivatives))
{
}

void DerivativeSimulator::runSimulations(int numThreads, int numSimulations, int numDays)
{
    assetData.clear();
    derivativeData.clear();
    assetData.resize(numThreads * numSimulations);
    derivativeData.resize(numThreads * numSimulations);
    for(int i = 0; i < derivativeData.size(); ++i)
    {
        derivativeData[i].resize(derivatives.size());
    }

    std::vector<std::thread> threads;

    for(int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(&DerivativeSimulator::simulate, this, i, numSimulations, numDays);
    }

    for(auto& thread : threads)
    {
        thread.join();
    }

    for(int simIndex = 0; simIndex < 3; ++simIndex)
    {
        for(int day = 0; day < numDays; ++day)
        {
               std::cout << "Day " << day << "  Asset Price " << *assetData[simIndex][day][AssetDataClass::Price]._Cast<double>() <<
                   "  Deriv Execution Value " << derivativeData[simIndex][0][day][DerivativeDataClass::Execution] <<
                   "  Deriv Accumulation Value " << derivativeData[simIndex][0][day][DerivativeDataClass::Accumulation] <<
                   "  Deriv Present Value " << derivativeData[simIndex][0][day][DerivativeDataClass::Total] << "\n";
        }
    }
}

bool DerivativeSimulator::hasAssetData(int simIndex, int day, AssetDataClass dataClass) const
{
    return assetData[simIndex].at(day).count(dataClass) == 1;
}

bool DerivativeSimulator::hasDerivativeData(int simIndex, int derivativeIndex, int day, DerivativeDataClass dataClass) const
{
    return derivativeData[simIndex][derivativeIndex].at(day).count(dataClass) == 1;

}

std::any DerivativeSimulator::getAssetData(int simIndex, int day, AssetDataClass dataClass) const
{
    return assetData[simIndex].at(day).at(dataClass);
}

double DerivativeSimulator::getDerivativeData(int simIndex, int derivativeIndex, int day, DerivativeDataClass dataClass) const
{
    return derivativeData[simIndex][derivativeIndex].at(day).at(dataClass);
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
        for(int derivativeIndex = 0; derivativeIndex < derivatives.size(); ++derivativeIndex)
        {
            derivatives[derivativeIndex].get()->update(0, model.getExpectedReturn(), assetData[simIndex][0]);
            derivativeData[simIndex][derivativeIndex][0] = derivatives[derivativeIndex].get()->getData();
        }
    
        for(int day = 1; day <= numDays; ++day)
        {
            model.advance();
            assetData[simIndex][day] = model.getData();
            for(int derivativeIndex = 0; derivativeIndex < derivatives.size(); ++derivativeIndex)
            {
                derivatives[derivativeIndex].get()->update(day, model.getExpectedReturn(), assetData[simIndex][day]);
                derivativeData[simIndex][derivativeIndex][day] = derivatives[derivativeIndex].get()->getData();
            }
        }
    }
}

extern "C"
{
    DerivativeSimulator* derivative_simulator_create(AssetModel* model, AssetDerivative** derivativePtrs, int derivativePtrsSize)
    {
        std::vector<std::unique_ptr<AssetDerivative>> derivatives;
        for(int derivative = 0; derivative < derivativePtrsSize; derivative++)
        {
            derivatives.push_back(std::unique_ptr<AssetDerivative>(derivativePtrs[derivative]));
        }

        return new DerivativeSimulator(*model, derivatives);
    }

    void derivative_simulator_delete(DerivativeSimulator* simulator)
    {
        delete simulator;
    }

    void derivative_simulator_run_simulations(DerivativeSimulator* simulator, int numThreads, int numSimulations, int numDays)
    {
        simulator->runSimulations(numThreads, numSimulations, numDays);
    }

    bool derivative_simulator_has_asset_data(DerivativeSimulator* simulator, int simIndex, int day, AssetDataClass dataClass)
    {
        return simulator->hasAssetData(simIndex, day, dataClass);
    }

    bool derivative_simulator_has_derivative_data(DerivativeSimulator* simulator, int simIndex, int derivativeIndex, int day, DerivativeDataClass dataClass)
    {
        return simulator->hasDerivativeData(simIndex, derivativeIndex, day, dataClass);
    }

    double derivative_simulator_get_asset_data_as_double(DerivativeSimulator* simulator, int simIndex, int day, AssetDataClass dataClass)
    {
        auto data = simulator->getAssetData(simIndex, day, dataClass);
        if(data.type() == typeid(double))
        {
            return std::any_cast<double>(data);
        }
    }

    int derivative_simulator_get_asset_data_as_int(DerivativeSimulator* simulator, int simIndex, int day, AssetDataClass dataClass)
    {
        auto data = simulator->getAssetData(simIndex, day, dataClass);
        if(data.type() == typeid(int))
        {
            return std::any_cast<int>(data);
        }
    }

    double derivative_simulator_get_derivative_data(DerivativeSimulator* simulator, int simIndex, int derivativeIndex, int day, DerivativeDataClass dataClass)
    {
        return simulator->getDerivativeData(simIndex, derivativeIndex, day, dataClass);
    }
}