from g4epy.console_run_sink import ConsoleRunSink
from g4epy.notebook_run_sink import NotebookRunSink
from .test_env import is_notebook
from .runner import run


class Geant4Eic(object):
    def __init__(self, gui='auto'):
        self.config = {}

        if not gui:
            self.is_notebook = False
            self.sink = ConsoleRunSink()
        else:
            self.is_notebook = is_notebook()
            if (gui == 'auto' and self.is_notebook) or gui == 'notebook':
                self.sink = NotebookRunSink()
            else:
                self.sink = ConsoleRunSink()

        self.runner = None
        self.exec_path = 'g4e'

    def source(self, source_strings, is_background=False):
        """Input source (usually files) configuration.

        This function stacks with plugins and another source

        ```python
        g4e.source('file1.txt')     \
           .source(['file2.txt', 'file3.txt'])
           .source('background.txt', is_background=True)     \
        ```
        """




        if isinstance(source_strings, str):
            self.config['input_files'].append(source_strings)
        else:
            self.config['input_files'].extend(source_strings)

        return self

    def reset_plugins(self):
        """
        Resets the plugin configuration
        """

        self.config['plugins'] = {}
        return self

    def configure_plugins(self, plugins):
        """ Configures plugins (using dicts). Replaces the previous configuration.

        :param plugins:
        :return: dict with plugins config
        """
        self.reset_plugins()

        if not plugins:
            return self.config['plugins']

        def _update(plugin_name, plugin_data):
            if plugin_name == 'jana' and plugin_data:
                self.config['params'] = plugin_data
            else:
                self.config['plugins'][plugin_name] = plugin_data

        assert isinstance(plugins, list)
        for item in plugins:
            if isinstance(item, str):
                _update(item, {})
            elif isinstance(item, tuple) or isinstance(item, list):
                # plugin in form:
                # ('name', {config})
                assert isinstance(item[1], dict)
                _update(item[0], item[1])
            else:
                assert isinstance(item, dict)
                # dict must be with one key like:
                # {'beagle_reader': { ... configs ... }}
                plugin_name = list(item)[0]
                _update(plugin_name, item[plugin_name])

        return self.config['plugins']

    def run(self):
        # if not self.sink.is_displayed:
        self.sink.display()
        command = f"{self.exec_path} {self.get_run_command()} -Pjana:debug_plugin_loading=1 "
        self.sink.show_running_command(command)
        run(command, self.sink)

    def get_run_command(self):
        add_plugins_str = "-Pplugins=" + ",".join(self.config['plugins'].keys())
        plugins_params_str = ""
        for plugin_name, plugin_params in self.config['plugins'].items():
            if plugin_params:
                for name, value in plugin_params.items():
                    # We have some "magic" jana flags like nskip and nthreads
                    if plugin_name == 'jana' and name in ['nevents', 'nskip', 'nthreads']:
                        plugins_params_str += f' -P{name}={value}'
                    else:
                        plugins_params_str += f' -P{plugin_name}:{name}={value}'

        params_str = " ".join([f'-P{name}={value}' for name, value in self.config['params'].items()])
        files_str = " ".join([file for file in self.config['input_files']])
        flags_str = " ".join([flag for flag in self.config['flags']])
        return f'{add_plugins_str} {plugins_params_str} {params_str}  {files_str} {flags_str}'

    def _repr_html_(self):
        plugins_str = ",".join(self.config['plugins'].keys())

        result_str = f"<strong>eJana</strong> configured<br><strong>plugins:</strong> {plugins_str}"
        if self.config['input_files']:
            sources_str = "<br>".join(self.config['input_files'])
            result_str += f"<br><strong>sources:</strong><br>{sources_str}"
        return result_str


if __name__ == "__main__":
    g4e = Geant4Eic()