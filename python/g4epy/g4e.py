import json
import os
import sys
import threading

from g4epy.console_run_sink import ConsoleRunSink
from g4epy.notebook_run_sink import NotebookRunSink
from .test_env import is_notebook
from .runner import run, do_execute, sink_printer, stream_subprocess, console_printer
from .mc import build_file_open_command, detect_mc_type, McFileTypes


class Geant4Eic(object):
    def __init__(self, **kwargs):
        self.config = {}
        self.user_commands = []    # Direct execution commands user added
        self.commands = []         # Total list of commands, generated by g4epy + user_commands
        self.user_macros = []      # a list of user macroses to run

        self.config.update(kwargs)  # update configs

        sink = self.config.get('sink', 'auto')
        if not sink:
            self.is_notebook = False
            self.sink = ConsoleRunSink()
        else:
            self.is_notebook = is_notebook()
            if (sink == 'auto' and self.is_notebook) or sink == 'notebook':
                self.sink = NotebookRunSink()
            else:
                self.sink = ConsoleRunSink()

        if 'G4E_HOME' not in os.environ:
            self.config['g4e_home'] = ''
            print("WARNING. G4E_HOME environment variable is not set. "
                  "Looking for g4e executable and all resource files in this directory. Which probably is an error")
        else:
            self.config['g4e_home'] = os.environ['G4E_HOME']

            default_build_prefix = os.path.join(os.environ['G4E_HOME'], 'cmake-build-debug')
            self.config['build_prefix'] = self.config.get('executable', default_build_prefix)

            default_path = os.path.join(os.environ['G4E_HOME'], 'cmake-build-debug', 'g4e')
            self.config['executable'] = self.config.get('executable', default_path)

        # set defaults if they are not set for the user
        self.config['output'] = self.config.get('output', 'test_run')
        self.config['source_file'] = self.config.get('source_file', '/home/romanov/Downloads/beagle_eD.txt')
        self.config['beamline'] = self.config.get('beamline', 'jleic')
        self.config['detector'] = self.config.get('detector', 'jleic')
        self.config['is_batch'] = self.config.get('is_batch', True)
        self.config['executable'] = self.config.get('executable', 'g4e')

        self.runner = None

    def macro(self, files):
        """
        Adds macro execution. Might be a single file name or a list of names
        :param files: Single file or many files
        :return: self
        """
        if isinstance(files, str):
            files = [files]         # just a single file

            files = files           # a list of files

        for file in files:
            self.command(f"/control/execute {file}")

        return self

    def command(self, value):
        """
        value is: string - execute string
        list of strings - execute all strings
        list of list of strings - each sublist is " ".join and executed as list of strings

        :param value:
        :return:
        """
        if isinstance(value, str):
            self.user_commands.append(value)
            return self

        # Now we assume that value is a list of something
        for item in value:
            if isinstance(item, str):
                self.user_commands.append(item)
            else:
                # this is a sublist
                self.user_commands.append(" ".join(item))

        return self

    def source(self, source_file):
        """Input source (usually files) configuration.

        This function stacks with plugins and another source

        ```python
        g4e.source('file1.txt')     \
           .source(['file2.txt', 'file3.txt'])
           .source('background.txt', is_background=True)     \
        ```
        """

        # Tries to identify MC file type and generate g4e messenger command to process it
        self.config['source_file'] = source_file

        mc_type = detect_mc_type(source_file)
        if mc_type == McFileTypes.UNKNOWN:
            raise ValueError(f"Unable to automatically identify the type of file. File name: '{source_file}'")
        source_cmd = build_file_open_command(mc_type, source_file)

        self.command(source_cmd)
        return self

    def save_run_context(self):
        run_context_file = self.config['output'] + ".run.json"
        self.config['run_context_file'] = run_context_file
        self.config['work_dir'] = os.getcwd()

        # What we are saving:
        data = {"config": self.config,
                "user_execs": self.user_commands}

        with open(run_context_file, "w") as f:
            json.dump(data, f, indent=4, sort_keys=True)

        return self

    def generate_run_mac(self):
        run_mac_file = self.config['output'] + ".run.mac"
        self.config['run_mac_file'] = run_mac_file
        # TODO make it a function

        # Base mac file is something like jleic.mac or jleicvis.mac
        # construct is with 'detector' and 'is_batch'
        det_name = self.config['detector']
        is_batch = self.config['is_batch']
        base_mac_file = det_name if is_batch else det_name + 'vis'
        base_mac_file += '.mac'

        self.commands.append(f"/control/execute {base_mac_file}")
        self.commands.extend(self.user_commands)

        with open(run_mac_file, "w") as f:
            f.write('\n'.join(self.commands) + '\n')

        return self

    def vis(self, run_vis=True):
        """Ensures visualization display is turned on"""
        self.config['is_batch'] = not run_vis
        return self

    def batch(self, run_batch=True):
        """Ensures geant runs in batch (no visualization) mode"""
        self.config['is_batch'] = not run_batch
        return self

    def run(self):
        """Runs the configuration"""

        # Check source file:
        if not self.config['source_file']:
            raise ValueError("No source file configured. Provide a source with source(...) function")

        self.generate_run_mac()
        self.save_run_context()

        # if not self.sink.is_displayed:
        self.sink.display()

        # Generate execution file
        command = f"{self.config['executable']} {self.get_run_command()} -o {self.config['output']}"

        self.sink.to_show = ["Event:", "Geant4 version", "Init"]
        self.sink.show_running_command(command)
        run(command, self.sink)

    def get_run_command(self):
        params = " {}".format(self.config['run_mac_file'])
        if not self.config['is_batch']:
            params += " --gui"

        return params

    def _repr_html_(self):
        result_str = f"<strong>g4e</strong> configured<br>"
        if self.config['source_file']:
            result_str += f"<br><strong>sources:</strong><br>{self.config['source_file']}"
        return result_str

    def beam_on(self, value):
        self.command(f"/run/beamOn {value}")
        return self

    def output(self, param):
        self.config['output'] = param
        return self


if __name__ == "__main__":
    g4e = Geant4Eic()