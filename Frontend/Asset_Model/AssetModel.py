import ctypes
from ctypes import POINTER, c_int, c_double, c_void_p
from enum import Enum

# Load the DLL
dll_path = r'D:\Projects\Monte-Carlo-Custom-Derivative-Pricing\Backend\Monte-Carlo-Custom-Derivative-Pricing\x64\Release\CustomDerivativePricing.dll'
cdp_lib = ctypes.CDLL(dll_path)

cdp_lib.asset_model_create.argtypes = [
    ctypes.c_int,               # numTradingDays
    ctypes.c_double,            # price
    ctypes.c_double,            # expectedReturn
    ctypes.c_double,            # volatility
    ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)),  # factorPtrs (pointer to an array of pointers)
    ctypes.c_int                # factorPtrsSize
]
cdp_lib.asset_model_create.restype = POINTER(c_void_p)  # Pointer to AssetModel

class AssetModel:
    """
    Class to interface with the AssetModel C++ class through ctypes.

    Attributes:
        _model_ptr (ctypes.POINTER(ctypes.c_void_p)): Pointer to the C++ AssetModel instance.
    """
    def __init__(self, num_trading_days: int, price: float, expected_return: float, volatility: float, factors: list):
        """
        Initializes the AssetModel object by creating an instance in the C++ library.

        Args:
            num_trading_days (int): Number of trading days.
            price (float): Initial price.
            expected_return (float): Expected return.
            volatility (float): Volatility.
            factors (list of VanillaOption): List of VanillaOption objects to be used as factors.
        """
        self._model_ptr = None

        factor_ptrs_lst = [factor._factor_ptr for factor in factors]
        factor_ptrs = (ctypes.POINTER(ctypes.c_void_p) * len(factors))(*factor_ptrs_lst)
        
        self._model_ptr = cdp_lib.asset_model_create(
            num_trading_days, price, expected_return, volatility, factor_ptrs, len(factors))
        
        if not self._model_ptr:
            raise Exception("Failed to create AssetModel object")

    def __del__(self):
        """
        Cleans up the C++ AssetModel instance by calling the asset_model_delete function.
        """
        if hasattr(self, '_model_ptr') and self._model_ptr:
            cdp_lib.asset_model_delete(self._model_ptr)
            self._model_ptr = None

