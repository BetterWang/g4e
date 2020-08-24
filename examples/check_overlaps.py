import inspect
from os import path
from g4epy import Geant4Eic

this_file_dir = path.dirname(inspect.stack()[0][1])

g4e = Geant4Eic()

g4e.output('overlaps')\
   .command('/run/initialize')\
   .command('/detsetup/checkOverlaps')\
   .run()