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
    def application_name(self) -> str:
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

    def __resize_callback(self, sender, data):
        width = dpg.get_item_width("MenuWindow")
        height = dpg.get_item_height("MenuWindow")

#        dpg.set_item_pos("MainTextGroup", (int(width * 0.35), int(height * 0.1)))

        dpg.set_item_width("CalendarButton", int(width * 0.3))
        dpg.set_item_height("CalendarButton", int(height * 0.1))
        dpg.set_item_pos("CalendarButton", (int(width * 0.35), int(height * 0.3)))

        dpg.set_item_width("ExitButton", int(width * 0.3))
        dpg.set_item_height("ExitButton", int(height * 0.1))
        dpg.set_item_pos("ExitButton", (int(width * 0.35), int(height * 0.5)))

    def build_window(self, *window_size: int) -> bool:

        if len(window_size) != 2:
            u.TRACE_LOG(u.LogLevel.LOG_LEVEL_ERR, "Application.py def build_window: window_size takes exactly two arguments.")
            return False

        application_name = self.application_name
        app_width, app_height = window_size

        dpg.create_context()
        dpg.create_viewport(title=application_name, width=app_width, height=app_height)

        with dpg.window(label="Main menu", tag="MenuWindow", width=app_width, height=app_height):
#            with dpg.group(tag="MainTextGroup"):
#                dpg.add_text("Main menu", tag="MainText")

            dpg.add_button(label="Calendar", tag="CalendarButton")
            dpg.add_button(label="Exit", tag="ExitButton", callback=self.on_exit_button)

        dpg.set_viewport_resize_callback(self.__resize_callback)

        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.set_primary_window("MenuWindow", True)

        self.__resize_callback(None, None)

        dpg.start_dearpygui()
        dpg.destroy_context()
        return True

    def on_exit_button(self):
        app.close()

    def run(self):
        self.BINARY_on_update()
        application_size = self.application_size
        self.build_window(*application_size)

    def on_update(self):
        pass

