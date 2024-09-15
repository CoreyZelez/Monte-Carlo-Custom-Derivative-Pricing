import sys
import os
import ctypes
from ctypes import POINTER, c_int, c_double, c_void_p
from enum import Enum

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'Asset_Model')))
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'Derivatives')))
sys.path.append(os.path.abspath(r'd:\Projects\Monte-Carlo-Custom-Derivative-Pricing\Frontend'))

for path in sys.path:
    print(path)

from Asset_Model.AssetModel import AssetModel 
from Asset_Model.FixedDividendFactor import FixedDividendFactor
from Derivatives.VanillaOption import *

# Load the DLL.
dll_path = r'D:\Projects\Monte-Carlo-Custom-Derivative-Pricing\Backend\Monte-Carlo-Custom-Derivative-Pricing\x64\Release\CustomDerivativePricing.dll'
cdp_lib = ctypes.CDLL(dll_path)

# Define the argument and return types for the functions in the DLL
cdp_lib.derivative_simulator_create.argtypes = [ctypes.POINTER(c_void_p), ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)), c_int]
cdp_lib.derivative_simulator_create.restype = ctypes.POINTER(c_void_p)

cdp_lib.derivative_simulator_run_simulations.argtypes = [ctypes.POINTER(c_void_p), c_int, c_int, c_int]
cdp_lib.derivative_simulator_run_simulations.restype = None

cdp_lib.derivative_simulator_has_asset_data.argtypes = [ctypes.POINTER(c_void_p), c_int, c_int, c_int]
cdp_lib.derivative_simulator_has_asset_data.restype = c_int

cdp_lib.derivative_simulator_has_derivative_data.argtypes = [ctypes.POINTER(c_void_p), c_int, c_int, c_int, c_int]
cdp_lib.derivative_simulator_has_derivative_data.restype = c_int

cdp_lib.derivative_simulator_get_asset_data_as_double.argtypes = [ctypes.POINTER(c_void_p), c_int, c_int, c_int]
cdp_lib.derivative_simulator_get_asset_data_as_double.restype = c_double

cdp_lib.derivative_simulator_get_asset_data_as_int.argtypes = [ctypes.POINTER(c_void_p), c_int, c_int, c_int]
cdp_lib.derivative_simulator_get_asset_data_as_int.restype = c_int

cdp_lib.derivative_simulator_get_derivative_data.argtypes = [ctypes.POINTER(c_void_p), c_int, c_int, c_int, c_int]
cdp_lib.derivative_simulator_get_derivative_data.restype = c_double

cdp_lib.derivative_simulator_delete.argtypes = [ctypes.POINTER(c_void_p)]
cdp_lib.derivative_simulator_delete.restype = None

# Define the AssetDataClass and DerivativeDataClass enums if not defined
class AssetDataClass(Enum):
    PRICE = 0
    VOLATILITY = 1
    DAY_VOLATILITY = 2
    DIVDIEND = 3
    DIVIDEND_ANNOUNCEMENT = 4

class DerivativeDataClass(Enum):
    # Define values as needed
    EXAMPLE = 0

class DerivativeSimulator:
    """
    Class to interface with the DerivativeSimulator C++ class through ctypes.

    Attributes:
        _simulator_ptr: Pointer to the C++ DerivativeSimulator instance.
    """
    def __init__(self, model: AssetModel, derivatives: list):
        """
        Initializes the DerivativeSimulator object by creating an instance in the C++ library.

        Args:
            model: An instance of AssetModel to be used in simulations.
            derivatives: A list of AssetDerivative instances.
        """
        self._simulator_ptr = None

        derivative_ptrs_lst = [derivative._derivative_ptr for derivative in derivatives]
        derivative_ptrs = (ctypes.POINTER(ctypes.c_void_p) * len(derivatives))(*derivative_ptrs_lst)

        self._simulator_ptr = cdp_lib.derivative_simulator_create(
            model._model_ptr, derivative_ptrs, len(derivatives)
        )
        
        if not self._simulator_ptr:
            raise Exception("Failed to create DerivativeSimulator object")

    def run_simulations(self, num_threads: int, num_simulations: int, num_days: int):
        """
        Runs simulations using the specified parameters.

        Args:
            num_threads: Number of threads to use.
            num_simulations: Number of simulations per thread.
            num_days: Number of days to simulate.
        """
        cdp_lib.derivative_simulator_run_simulations(self._simulator_ptr, num_threads, num_simulations, num_days)

    def has_asset_data(self, sim_index: int, day: int, data_class: AssetDataClass) -> bool:
        """
        Checks if asset data is available for the given simulation index, day, and data class.

        Args:
            sim_index: Simulation index.
            day: Day of the simulation.
            data_class: Data class to check.

        Returns:
            bool: True if data is available, False otherwise.
        """
        return bool(cdp_lib.derivative_simulator_has_asset_data(self._simulator_ptr, sim_index, day, data_class.value))

    def has_derivative_data(self, sim_index: int, derivative_index: int, day: int, data_class: DerivativeDataClass) -> bool:
        """
        Checks if derivative data is available for the given simulation index, derivative index, day, and data class.

        Args:
            sim_index: Simulation index.
            derivative_index: Derivative index.
            day: Day of the simulation.
            data_class: Data class to check.

        Returns:
            bool: True if data is available, False otherwise.
        """
        return bool(cdp_lib.derivative_simulator_has_derivative_data(self._simulator_ptr, sim_index, derivative_index, day, data_class.value))

    def get_asset_data_as_double(self, sim_index: int, day: int, data_class: AssetDataClass) -> float:
        """
        Retrieves asset data as a double value.

        Args:
            sim_index: Simulation index.
            day: Day of the simulation.
            data_class: Data class to retrieve.

        Returns:
            float: The asset data value.
        """
        return cdp_lib.derivative_simulator_get_asset_data_as_double(self._simulator_ptr, sim_index, day, data_class.value)

    def get_asset_data_as_int(self, sim_index: int, day: int, data_class: AssetDataClass) -> int:
        """
        Retrieves asset data as an integer value.

        Args:
            sim_index: Simulation index.
            day: Day of the simulation.
            data_class: Data class to retrieve.

        Returns:
            int: The asset data value.
        """
        return cdp_lib.derivative_simulator_get_asset_data_as_int(self._simulator_ptr, sim_index, day, data_class.value)

    def get_derivative_data(self, sim_index: int, derivative_index: int, day: int, data_class: DerivativeDataClass) -> float:
        """
        Retrieves derivative data as a double value.

        Args:
            sim_index: Simulation index.
            derivative_index: Derivative index.
            day: Day of the simulation.
            data_class: Data class to retrieve.

        Returns:
            float: The derivative data value.
        """
        return cdp_lib.derivative_simulator_get_derivative_data(self._simulator_ptr, sim_index, derivative_index, day, data_class.value)

    def __del__(self):
        """
        Cleans up the C++ DerivativeSimulator instance by calling the derivative_simulator_delete function.
        """
        if hasattr(self, '_simulator_ptr') and self._simulator_ptr:
            cdp_lib.derivative_simulator_delete(self._simulator_ptr)
            self._simulator_ptr = None

if __name__ == "__main__":
    
    factors = [
        FixedDividendFactor(5, 10, 20),
        FixedDividendFactor(50, 70, 30)
    ]
    
    num_trading_days = 252
    price = 200
    expected_return = 0.05
    volatility = 0.2
    model = AssetModel(num_trading_days, price, expected_return, volatility, factors)  # Create your AssetModel instance
    derivatives = [
        VanillaOption(num_trading_days, VanillaOptionType.CALL, VanillaOptionStyle.AMERICAN, 210, 190),
        VanillaOption(num_trading_days, VanillaOptionType.PUT, VanillaOptionStyle.EUROPEAN, 210, 190)
    ]  

    cdp_lib.get_day(model._model_ptr)

    simulator = DerivativeSimulator(model, derivatives)
    simulator.run_simulations(num_threads=1, num_simulations=10000, num_days=100)
    print("DerivativeSimulator object created and simulations run successfully.")
