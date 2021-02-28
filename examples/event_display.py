import inspect
from os import path
from g4epy import Geant4Eic

g4e = Geant4Eic()

g4e.beam_on(0)\
   .vis()\
   .run()
