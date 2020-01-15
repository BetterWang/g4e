class ConsoleRunSink:

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