import dearpygui.dearpygui as dpg
import numpy as np

# Sample data
time = np.arange(100)
stock1 = np.sin(time / 10) * 50 + 100
stock2 = np.cos(time / 10) * 30 + 100

# Initialize Dear PyGui
dpg.create_context()

# Create a window
with dpg.handler_registry():
    dpg.add_key_press_handler(key=dpg.mvKey_Escape, callback=lambda: dpg.stop_dearpygui())

with dpg.window(label="Stock Price Chart"):
    # Create a plot
    with dpg.plot(label="Stock Prices", height=400, width=600):
        dpg.add_plot_legend()
        x_axis = dpg.add_plot_axis(dpg.mvXAxis, label="Time")
        y_axis = dpg.add_plot_axis(dpg.mvYAxis, label="Price")
        
        # Add series for stock 1
        dpg.add_line_series(x_axis, y_axis, time, stock1, label="Stock 1", color=(255, 0, 0))
        
        # Add series for stock 2
        dpg.add_line_series(x_axis, y_axis, time, stock2, label="Stock 2", color=(0, 0, 255))

# Show the window
dpg.create_viewport(title='Stock Price Time Series', width=800, height=600)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
