from g4epy import Geant4Eic

g4e = Geant4Eic()

g4e.source('eC_10x41_GCF_QE_Egaus_small.txt')\
   .output('test_run')\
   .beam_on(1000)\
   .run()
