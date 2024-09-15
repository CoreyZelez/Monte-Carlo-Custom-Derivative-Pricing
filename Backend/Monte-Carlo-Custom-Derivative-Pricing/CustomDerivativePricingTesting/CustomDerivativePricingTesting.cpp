#include "AssetModel.h"
#include "DerivativeSimulator.h"
#include "FixedDividendFactor.h"
#include "VanillaOption.h"
#include "AssetDerivative.h"
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<AssetDerivative> vanillaCall = std::make_unique<VanillaOption>(252, VanillaOption::OptionType::Call, VanillaOption::OptionStyle::American, 200, 50);
    std::unique_ptr<AssetDerivative> vanillaPut = std::make_unique<VanillaOption>(252, VanillaOption::OptionType::Put, VanillaOption::OptionStyle::American, 270, 50);

    std::vector<std::unique_ptr<AssetDerivative>> derivatives;
    derivatives.push_back(std::move(vanillaCall));
    derivatives.push_back(std::move(vanillaPut));

    std::unique_ptr<AssetModelFactor> fixedDividend = std::make_unique<FixedDividendFactor>(10, 30, 10);
    std::vector<std::unique_ptr<AssetModelFactor>> factors;
    factors.push_back(std::move(fixedDividend));
    
    AssetModel assetModel(252, 50, 0.1, 0.2, factors);
    DerivativeSimulator simulator(assetModel, derivatives);
    
    simulator.runSimulations(1, 4000, 300);
}