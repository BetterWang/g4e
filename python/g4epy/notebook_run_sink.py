import shlex
from IPython.core.display import display
from ipywidgets import widgets


class NotebookRunSink:

    def __init__(self):
        self._output_widget = widgets.Output()
        self._is_displayed = True
        self._label = widgets.Label(value="Initializing...")
        self._stop_button = widgets.Button(
            description='Terminate',
            disabled=False,
            button_style='',  # 'success', 'info', 'warning', 'danger' or ''
            tooltip='Terminate jana process',
            # icon='check'
        )
        self._command_label = widgets.HTML()

        self.to_show = []

    # noinspection PyTypeChecker
    def display(self):
        # title_widget = widgets.HTML('<em>Vertical Box Example</em>')
        self._stop_button.layout.display = ''
        control_box = widgets.HBox([self._stop_button, self._label])

        accordion = widgets.Accordion(children=[self._output_widget, self._command_label], selected_index=None)
        accordion.set_title(0, 'Full log')
        accordion.set_title(1, 'Run command')

        vbox = widgets.VBox([control_box, accordion])

        # display(accordion)
        display(vbox)
        self._is_displayed = True

    def add_line(self, line):
        tokens = line.split('\n')
        for token in tokens:
            for test in self.to_show:
                if test in token:
                    self._label.value = token

        self._output_widget.append_stdout(line + '\n')

    def done(self):
        self._stop_button.layout.display = 'none'

    def show_running_command(self, command):
        tokens = shlex.split(command)
        self._command_label.value = '<br>'.join(tokens)

    @property
    def is_displayed(self):
        return self._is_displayed