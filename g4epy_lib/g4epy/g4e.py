import json

from g4epy.console_run_sink import ConsoleRunSink
from g4epy.notebook_run_sink import NotebookRunSink
from .test_env import is_notebook
from .runner import run


class Geant4Eic(object):
    def __init__(self, gui='auto'):
        self.config = {}
        self.user_commands = []    # Direct execution commands user added
        self.commands = []         #

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
        self.config['output'] = 'test_run'
        self.config['input_file'] = '/home/romanov/Downloads/beagle_eD.txt'
        self.config['beamline'] = 'jleic'
        self.config['detector'] = 'jleic'
        self.config['is_batch'] = True
        self.config['executable_path'] = 'g4e'

    def execute(self, value):
        """
        value is: string - execute string
        list of strings - execute all strings
        list of list of strings - each sublist is " ".join and executed as list of strings

        :param value:
        :return:
        """
        if isinstance(value, str):
            self.user_commands.append(str)
            return

        # Now we assume that value is a list of something
        for item in value:
            if isinstance(value, str):
                self.user_commands.append(str)
            else:
                # this is a sublist
                self.user_commands.append(" ".join(item))

    def source(self, source_strings, is_background=False):
        """Input source (usually files) configuration.

        This function stacks with plugins and another source

        ```python
        g4e.source('file1.txt')     \
           .source(['file2.txt', 'file3.txt'])
           .source('background.txt', is_background=True)     \
        ```
        """

        destination = 'background_file' if is_background else 'input_file'

        if isinstance(source_strings, str):
            self.config[destination].append(source_strings)
        else:
            self.config[destination].extend(source_strings)

        return self

    def save_run_context(self):
        run_context_file = self.config['output'] + ".run.json"
        self.config['run_context_file'] = run_context_file

        # What we are saving:
        data = {"config": self.config,
                "user_execs": self.user_commands}

        with open(run_context_file, "w") as f:
            json.dump(data, f)

        return self

    def generate_run_mac(self):
        run_mac_file = self.config['output'] + ".run.mac"
        self.config['run_mac_file'] = run_mac_file
        # TODO make it a function

        # Base mac file is something like jleic.mac or jleicvis.mac
        # construct is with 'detector' and 'is_batch'
        det_name = self.config['detector']
        is_batch = self.config['is_batch']
        base_mac_file = det_name if is_batch else is_batch + 'vis'
        base_mac_file += '.mac'

        self.commands.append(f"/control/execute {base_mac_file}")
        self.commands.extend(self.user_commands)

        with open(run_mac_file, "w") as f:
            f.writelines(self.commands)

        return self

    def run(self):

        self.generate_run_mac()
        self.save_run_context()

        # if not self.sink.is_displayed:
        self.sink.display()
        command = f"{self.config['executable_path']} {self.get_run_command()}"
        self.sink.show_running_command(command)
        run(command, self.sink)

    def get_run_command(self):
        params = " {}".format(self.config['run_mac_file'])
        if not self.config['is_batch']:
            params += " --gui"

        return params


        # add_plugins_str = "-Pplugins=" + ",".join(self.config['plugins'].keys())
        # plugins_params_str = ""
        # for plugin_name, plugin_params in self.config['plugins'].items():
        #     if plugin_params:
        #         for name, value in plugin_params.items():
        #             # We have some "magic" jana flags like nskip and nthreads
        #             if plugin_name == 'jana' and name in ['nevents', 'nskip', 'nthreads']:
        #                 plugins_params_str += f' -P{name}={value}'
        #             else:
        #                 plugins_params_str += f' -P{plugin_name}:{name}={value}'
        #
        # params_str = " ".join([f'-P{name}={value}' for name, value in self.config['params'].items()])
        # files_str = " ".join([file for file in self.config['input_files']])
        # flags_str = " ".join([flag for flag in self.config['flags']])
        # return f'{add_plugins_str} {plugins_params_str} {params_str}  {files_str} {flags_str}'

    def _repr_html_(self):
        plugins_str = ",".join(self.config['plugins'].keys())

        result_str = f"<strong>eJana</strong> configured<br><strong>plugins:</strong> {plugins_str}"
        if self.config['input_files']:
            sources_str = "<br>".join(self.config['input_files'])
            result_str += f"<br><strong>sources:</strong><br>{sources_str}"
        return result_str


if __name__ == "__main__":
    g4e = Geant4Eic()