import unittest
import subprocess

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
        subprocess.run(["ls", "-l", "/dev/null"], capture_output=True)
        CompletedProcess(args=['ls', '-l', '/dev/null'], returncode=0,
                 stdout=b'crw-rw-rw- 1 root root 1, 3 Jan 23 16:23 /dev/null\n', stderr=b'')