// CustomDerivativePricing.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "CustomDerivativePricing.h"


// This is an example of an exported variable
CUSTOM_DERIVATIVE_PRICING_API int nCustomDerivativePricing=0;

// This is an example of an exported function.
CUSTOM_DERIVATIVE_PRICING_API int fnCustomDerivativePricing(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CCustomDerivativePricing::CCustomDerivativePricing()
{
    return;
}
