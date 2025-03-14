# app imported module from binary

import app

class Application:
    def __init__(self):
        self.__app_size = []

        self.__is_running = False

    def __repr__(self):
        return "{}"

    def __str__(self):
        return "{}"

    @property
    def application_status(self):
        return self.__is_running

    @property
    def app_size(self) -> list:
        return self.__app_size

    def on_update_binary(self):
        self.__is_running = app.get_application_status()
        self.__app_size = app.app_getApplicationSize()

    def run(self):
        self.on_update_binary()
        while self.__is_running:
            pass

    def on_update(self):
        pass

    def __del__(self):
        print(f"Deleted: {self.__app_height}")
