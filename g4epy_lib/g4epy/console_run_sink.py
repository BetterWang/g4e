class ConsoleRunSink:

    def __init__(self):
        self.to_show = []

    def add_line(self, line):
        print(line)

    def display(self):
        print("Rendering in console")

    def done(self):
        pass

    def show_running_command(self, command):
        print(f'Command = "{command}"')

    @property
    def is_displayed(self):
        return True