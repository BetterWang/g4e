import json
import os
import sys
import threading

from g4epy.console_run_sink import ConsoleRunSink
from .test_env import is_notebook
from .runner import run, do_execute, sink_printer, stream_subprocess, console_printer
from .mc import build_file_open_command, detect_mc_type, McFileTypes


class Geant4Eic(object):
    def __init__(self, **kwargs):
        """

        :param executable: Full path to g4e executable (if not set env variable 'G4E_HOME' is used to determine g4e path)
        :param build_prefix: see 'Building g4e with python`
        :param beamline: Beamline ip6 or ip8
        :param detector: Main detector construction name. Default 'refdet'
        :param is_batch: True - no GUI, False - Geant4 event display
        :param thread_count: Number of Threads (parallel workers)

        :param kwargs:
        """
        self.config = {}
        self.commands = []         # Total list of commands, generated by g4epy + user_commands
        self._user_commands_log = []

        self.config.update(kwargs)  # update configs

        sink = self.config.get('sink', 'auto')
        if not sink:
            self.is_notebook = False
            self.sink = ConsoleRunSink()
        else:
            self.is_notebook = is_notebook()
            if (sink == 'auto' and self.is_notebook) or sink == 'notebook':
                from g4epy.notebook_run_sink import NotebookRunSink   # Req IPython which might be not installed
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

            default_path = os.path.join(os.environ['G4E_HOME'], 'bin', 'g4e')
            self.config['executable'] = self.config.get('executable', default_path)

        # set defaults if they are not set for the user
        self.config['output'] = self.config.get('output', 'noname_run')

        self.config['beamline'] = self.config.get('beamline', 'ip6')
        self.config['detector'] = self.config.get('detector', 'refdet')
        self.config['is_batch'] = self.config.get('is_batch', True)
        self.config['executable'] = self.config.get('executable', 'g4e')
        self.config['thread_count'] = self.config.get('treads', 1)

        # Base mac file is something like jleic.mac
        # construct is with 'detector' and 'beamline'
        base_mac_file = self.config['detector'] + '.mac'
        self.commands.append(f"/control/execute {base_mac_file}")
        self.commands.append(f"/eic/beamline/name {self.config['beamline']}")

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
        def add_command(value):
            self._user_commands_log.append(value)
            self.commands.append(value)

        if isinstance(value, str):
            add_command(value)
            return self

        # Now we assume that value is a list of something
        for item in value:
            if isinstance(item, str):
                add_command(item)
            else:
                # this is a sublist
                add_command(" ".join(item))

        return self

    def source(self, source_file):
        """Input source configuration (usually file names).

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
                "user_commands": self._user_commands_log}

        with open(run_context_file, "w") as f:
            json.dump(data, f, indent=4, sort_keys=True)

        return self

    def generate_run_mac(self):
        run_mac_file = self.config['output'] + ".run.mac"
        self.config['run_mac_file'] = run_mac_file
        # TODO make it a function

        with open(run_mac_file, "w") as f:
            f.write('\n'.join(self.commands) + '\n')

        return self

    def vis(self, run_vis=True):
        """Ensures visualization display is turned on"""
        if run_vis:
            vis_mac_file = self.config['detector'] + '.vis.mac'
            self.commands.append(f"/control/execute {vis_mac_file}")
        self.config['is_batch'] = not run_vis
        return self

    def batch(self, run_batch=True):
        """Ensures geant runs in batch (no visualization) mode"""
        self.config['is_batch'] = not run_batch
        return self

    def threads(self, thread_count=1):
        """Number of Threads (parallel workers)"""
        self.config['thread_count'] = int(thread_count)
        return self

    def run(self):
        """Runs the configuration"""

        # Check source file:
        if 'source_file' not in self.config:
            print("WARNING not source file is given")
                    #raise ValueError("No source file configured. Provide a source with source(...) function")

        self.generate_run_mac()
        self.save_run_context()

        # if not self.sink.is_displayed:
        self.sink.display()

        if self.config['thread_count'] != 1:
            threads_flag = f"-t{self.config['thread_count']}"
        else:
            threads_flag = ''

        # Generate execution file
        command = f"{self.config['executable']} {self.get_run_command()} {threads_flag} -o {self.config['output']}"

        self.sink.to_show = ["Event", "Geant4 version", "Init", "Error", "Fatal", "Exception"]
        self.sink.show_running_command(command)
        return run(command, self.sink)

    def get_run_command(self):
        """Returns the command that will be used to run g4e"""
        params = " {}".format(self.config['run_mac_file'])
        if not self.config['is_batch']:
            params += " --gui"

        return params

    def _repr_html_(self):
        result_str = f"<strong>g4e</strong> configured<br>"

        if self.config.get('source_file'):
            result_str += f"<br><strong>sources: </strong><br>{self.config['source_file']}"
        if self.config.get('detector'):
            result_str += f"<br><strong>detector: </strong><br>{self.config['detector']}"
        if self.config.get('beamline'):
            result_str += f"<br><strong>beamline: </strong><br>{self.config['beamline']}"
        return result_str

    def beam_on(self, value):
        """Sets the number of events to process"""

        # vis automatically have to do /run/initialize
        if self.config['is_batch']:
            self.command("/run/initialize")
        self.command(f"/run/beamOn {value}")
        return self

    def output(self, param):
        """Sets base name of output files (G4E produces a number of files with different extensions)"""
        self.config['output'] = param
        return self


if __name__ == "__main__":
    g4e = Geant4Eic()