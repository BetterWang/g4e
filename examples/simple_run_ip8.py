import inspect
from os import path
from g4epy import Geant4Eic

g4e = Geant4Eic(beamline='ip8')

#file location https://gitlab.com/eic/escalate/workspace/-/blob/master/data/herwig6_20k.zip
g4e.source('herwig6_20k.hepmc') \
    .output('test_run') \
    .beam_on(2000) \
    .run()
