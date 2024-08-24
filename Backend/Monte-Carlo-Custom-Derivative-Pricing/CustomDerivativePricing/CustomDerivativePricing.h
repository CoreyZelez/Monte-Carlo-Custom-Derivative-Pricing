#ifdef CUSTOM_DERIVATIVE_PRICING_API_EXPORTS
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllexport)
#else
#define CUSTOM_DERIVATIVE_PRICING_API __declspec(dllimport)
#endif

// This class is exported from the dll
class CUSTOM_DERIVATIVE_PRICING_API CCustomDerivativePricing {
public:
	CCustomDerivativePricing(void);
	// TODO: add your methods here.
};

extern CUSTOM_DERIVATIVE_PRICING_API int nCustomDerivativePricing;

CUSTOM_DERIVATIVE_PRICING_API int fnCustomDerivativePricing(void);
