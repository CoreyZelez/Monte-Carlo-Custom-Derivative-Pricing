// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the CUSTOMDERIVATIVEPRICING_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// CUSTOMDERIVATIVEPRICING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CUSTOMDERIVATIVEPRICING_EXPORTS
#define CUSTOMDERIVATIVEPRICING_API __declspec(dllexport)
#else
#define CUSTOMDERIVATIVEPRICING_API __declspec(dllimport)
#endif

// This class is exported from the dll
class CUSTOMDERIVATIVEPRICING_API CCustomDerivativePricing {
public:
	CCustomDerivativePricing(void);
	// TODO: add your methods here.
};

extern CUSTOMDERIVATIVEPRICING_API int nCustomDerivativePricing;

CUSTOMDERIVATIVEPRICING_API int fnCustomDerivativePricing(void);
