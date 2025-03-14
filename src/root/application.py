import app

class Application:
    def __init__(self):
        self.__app_width = 0
        self.__app_height = 0
        self.__is_running = False

    def __repr__(self):
        return 

    @property
    def application_status(self):
        return self.__is_running

    @property
    def height(self) -> int:
        return self.app_height

    @property
    def width(self) -> int:
        return self.app_width

    def on_update_binary(self):
        self.__is_running = app.get_application_status()
        self.__app_width = app.get_width()
        self.__app_height = app.get_height()

    def run(self):
        on_update_binary()
        while self.__is_running:
            pass

    def on_update(self):
        pass
            
