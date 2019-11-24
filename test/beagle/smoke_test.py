import inspect
import sys
import unittest
import subprocess
import os

class BeagleSmokeTest(unittest.TestCase):
    def setUp(self):
        pass
        # self.widget = Widget('The widget')

    # def test_default_widget_size(self):
    #     self.assertEqual(self.widget.size(), (50,50),
    #                      'incorrect default size')
    #
    # def test_widget_resize(self):
    #     self.widget.resize(100,150)
    #     self.assertEqual(self.widget.size(), (100,150),
    #                      'wrong size after resize')

    def test_beagle_1_event(self):
        this_script_dir = os.path.dirname(inspect.stack()[0][1])
        os.chdir(this_script_dir)
        result = subprocess.run(["/home/romanov/eic/g4e/g4e-dev/cmake-build-debug/g4e", "beagle_io.mac"],
                        check=True,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)

        print(str(result.stdout, 'utf-8'))