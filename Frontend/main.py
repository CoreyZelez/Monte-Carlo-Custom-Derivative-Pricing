import sys
import os
import ctypes
import dearpygui.dearpygui as dpg
from Asset_Model.AssetModel import AssetModel 
from Asset_Model.FixedDividendFactor import FixedDividendFactor
from Derivatives.VanillaOption import *
from Simulation.DerivativeSimulator import *

factors = [
    FixedDividendFactor(5, 10, 30),
    FixedDividendFactor(50, 70, 50)
]
    
num_trading_days = 252
price = 200
expected_return = 0.2
volatility = 0.1
model = AssetModel(num_trading_days, price, expected_return, volatility, factors)  # Create your AssetModel instance
derivatives = [
    VanillaOption(num_trading_days, VanillaOptionType.CALL, VanillaOptionStyle.AMERICAN, 210, 190),
    VanillaOption(num_trading_days, VanillaOptionType.PUT, VanillaOptionStyle.EUROPEAN, 210, 190)
]  

simulator = DerivativeSimulator(model, derivatives)
simulator.run_simulations(num_threads=4, num_simulations=100, num_days=2000)

x_data = []
y_data_1 = []
y_data_2 = []
y_data_3 = []
y_data_4 = []
y_data_5 = []

for day in range(2000):
    x_data.append(day)
    y_data_1.append(simulator.get_asset_data_as_double(0, day, AssetDataClass.PRICE))
    y_data_2.append(simulator.get_asset_data_as_double(1, day, AssetDataClass.PRICE))
    y_data_3.append(simulator.get_asset_data_as_double(2, day, AssetDataClass.PRICE))
    y_data_4.append(simulator.get_asset_data_as_double(3, day, AssetDataClass.PRICE))
    y_data_5.append(simulator.get_asset_data_as_double(4, day, AssetDataClass.PRICE))

# Create the context for the GUI
dpg.create_context()

# Create the main window
with dpg.window(label="Time Series Charts", width=800, height=800):

    # Create first row
    with dpg.group(horizontal=True):
        # First plot (Top-left) with multiple time series
        with dpg.plot(label="Time Series 1 (Multiple)", height=300, width=350):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="X Axis")
            with dpg.plot_axis(dpg.mvYAxis, label="Y Axis"):
                dpg.add_line_series(x_data, y_data_1, label="Series 1")
                dpg.add_line_series(x_data, y_data_5, label="Series 2")

        # Second plot (Top-right)
        with dpg.plot(label="Time Series 2", height=300, width=350):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="X Axis")
            with dpg.plot_axis(dpg.mvYAxis, label="Y Axis"):
                dpg.add_line_series(x_data, y_data_2, label="Series 2")

    # Create second row
    with dpg.group(horizontal=True):
        # Third plot (Bottom-left)
        with dpg.plot(label="Time Series 3", height=300, width=350):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="X Axis")
            with dpg.plot_axis(dpg.mvYAxis, label="Y Axis"):
                dpg.add_line_series(x_data, y_data_3, label="Series 3")

        # Fourth plot (Bottom-right)
        with dpg.plot(label="Time Series 4", height=300, width=350):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="X Axis")
            with dpg.plot_axis(dpg.mvYAxis, label="Y Axis"):
                dpg.add_line_series(x_data, y_data_4, label="Series 4")

# Start the Dear PyGui event loop
dpg.create_viewport(title='Four Charts Example', width=800, height=800)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
