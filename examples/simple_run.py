import inspect
from os import path
from g4epy import Geant4Eic

this_file_dir = path.dirname(inspect.stack()[0][1])

g4e = Geant4Eic()

g4e.source('/home/romanov/Downloads/herwig6_20k.hepmc')\
   .output('test_run')\
   .beam_on(2000)\
   .run()
