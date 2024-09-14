import ctypes
from ctypes import POINTER, c_int, c_double, c_void_p

# Load the DLL
dll_path = r'D:\Projects\Monte-Carlo-Custom-Derivative-Pricing\Backend\Monte-Carlo-Custom-Derivative-Pricing\x64\Release\CustomDerivativePricing.dll'
cdp_lib = ctypes.CDLL(dll_path)

class FixedDividendFactor:
    """
    Class to interface with the FixedDividendFactor C++ class through ctypes.

    Attributes:
        _factor_ptr (ctypes.POINTER(ctypes.c_void_p)): Pointer to the C++ FixedDividendFactor instance.
    """
    def __init__(self, announcement_day: int, payment_day: int, dividend_amount: float):
        """
        Initializes the FixedDividendFactor object by creating an instance in the C++ library.

        Args:
            announcement_day (int): Day when the dividend is announced.
            payment_day (int): Day when the dividend is paid.
            dividend_amount (float): Amount of the dividend.
        """
        self._factor_ptr = None
        
        # Set argument types and return type.
        cdp_lib.fixed_dividend_factor_create.argtypes = [c_int, c_int, c_double]
        cdp_lib.fixed_dividend_factor_create.restype = POINTER(c_void_p)
        
        # Create the FixedDividendFactor instance.
        self._factor_ptr = cdp_lib.fixed_dividend_factor_create(
            announcement_day, payment_day, dividend_amount)
        
        if not self._factor_ptr:
            raise Exception("Failed to create FixedDividendFactor object")

    def __del__(self):
        """
        Cleans up the C++ FixedDividendFactor instance by calling the fixed_dividend_factor_delete function.
        """
        if hasattr(self, '_factor_ptr') and self._factor_ptr:
            cdp_lib.fixed_dividend_factor_delete(self._factor_ptr)
            self._factor_ptr = None

# Example usage
if __name__ == "__main__":
    # Create FixedDividendFactor object
    announcement_day = 15
    payment_day = 30
    dividend_amount = 5.0

    factor = FixedDividendFactor(announcement_day, payment_day, dividend_amount)
    print("FixedDividendFactor object created successfully.")