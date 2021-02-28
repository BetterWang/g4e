# This file demonstrates how to pass Geant4 macro commands using command function
import inspect
from os import path
from g4epy import Geant4Eic

g4e = Geant4Eic()

# Single string command
g4e.command('/eic/detector refdet')

# HepMC doesn't necessarily provides beam energy information, so we can set it manually
g4e.command(['/eic/refdet/eBeam 5',
             '/eic/refdet/pBeam 50'
             ])

#file location https://gitlab.com/eic/escalate/workspace/-/blob/master/data/herwig6_20k.zip
g4e.source('herwig6_20k.hepmc') \
   .output('test_run')\
   .beam_on(2000)\
   .run()
