import inspect
import os
import subprocess
import unittest

from g4epy.mc import McFileTypes, build_file_open_command


class McFileIdentificationTest(unittest.TestCase):
    def setUp(self):
        this_script_dir = os.path.dirname(inspect.stack()[0][1])

    def test_identify_files(self):
        command = build_file_open_command(McFileTypes.PYTHIA_LUND, "some_file")
        self.assertTrue('pythiaAscii' in command[0])
        self.assertTrue('some_file' in command[1])

    def test_unsupported_file(self):
        self.assertRaises(NotImplementedError, build_file_open_command, McFileTypes.UNKNOWN, "some_file")
