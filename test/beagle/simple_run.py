import inspect
from os import path

import sys

from g4epy import Geant4Eic
import tempfile

this_file_dir = path.dirname(inspect.stack()[0][1])

g4e = Geant4Eic(detector='jleic')
g4e.source('beagle_test_data.txt').output('beagle_smoke_test')

if "batch" not in sys.argv:
    g4e.vis()

g4e.beam_on(1)
g4e.run()

