import ctypes
from enum import Enum

# Load the DLL
dll_path = r'D:\Projects\Monte-Carlo-Custom-Derivative-Pricing\Backend\Monte-Carlo-Custom-Derivative-Pricing\x64\Release\CustomDerivativePricing.dll'
cdp_lib = ctypes.CDLL(dll_path)

class VanillaOptionType(Enum):
    CALL = 0
    PUT = 1

class VanillaOptionStyle(Enum):
    AMERICAN = 0
    EUROPEAN = 1

cdp_lib.vanilla_option_create.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_double]
cdp_lib.vanilla_option_create.restype = ctypes.POINTER(ctypes.c_void_p) 

cdp_lib.vanilla_option_delete.argtypes = [ctypes.POINTER(ctypes.c_void_p)]
cdp_lib.vanilla_option_delete.restype = None

class VanillaOption:
    """
    Vanilla put or call option for usage with AssetModel.
    """
    def __init__(self, num_trading_days, option_type, option_style, expiry_day, strike_price):
        self._derivative_ptr = cdp_lib.vanilla_option_create(num_trading_days, option_type.value, option_style.value, expiry_day, strike_price)
        
        if not self._derivative_ptr:
            raise Exception("Failed to create VanillaOption object")
    
    def __del__(self):
        if self._derivative_ptr:
            cdp_lib.vanilla_option_delete(self._derivative_ptr)
            self._derivative_ptr = None

# Example usage:
if __name__ == "__main__":
    num_trading_days = 30
    option_type = VanillaOptionType.CALL
    option_style = VanillaOptionStyle.EUROPEAN
    expiry_day = 45
    strike_price = 100.0

    option = VanillaOption(num_trading_days, option_type, option_style, expiry_day, strike_price)
    print("VanillaOption object created successfully.")
    
