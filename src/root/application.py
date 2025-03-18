import app
import utils as u

import dearpygui.dearpygui as dpg

class Application:
    def __init__(self):
        self.__app_size = []
        self.__is_running = False

    def __repr__(self):
        return f"Application(self.__app_size={self.__app_size}, self.__is_running={self.__is_running})"

    @property
    def appliction_name(self) -> str:
        return app.get_application_name()

    @property
    def application_status(self) -> bool:
        return self.__is_running

    @property
    def application_size(self) -> list:
        return self.__app_size

    def BINARY_on_update(self):
        """
        update binary status /* load values from config... */
        """
        self.__is_running = app.get_application_status()
        self.__app_size = app.get_application_size()

        #   debug
        print(self.__app_size)

    def build_window(self, window_size: list) -> bool:

        if not window_size or not all(isinstance(x, int) for x in window_size):
            u.TRACE_LOG(u.LogLevel.LOG_LEVEL_ERR, "Application.py def build_window: window_size is null or does not contain int.")
            return False

        application_name = self.application_name

        app_width = window_size[0]
        app_height = window_size[1]

        dpg.create_context()
        dpg.create_viewport(title=application_name, width=app_width, height=app_height)

        with dpg.window(label="Example Window"):
            dpg.add_text("Hello, world")
            dpg.add_button(label="Save")
            dpg.add_input_text(label="string", default_value="Quick brown fox")
            dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.start_dearpygui()
        dpg.destroy_context()
        return True

    def run(self):
        self.BINARY_on_update()

        application_size = self.application_size

        self.build_window(application_size)

        while self.__is_running:
            pass

    def on_update(self):
        pass
            
