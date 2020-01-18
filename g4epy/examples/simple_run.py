from g4epy import Geant4Eic
import tempfile


g4e = Geant4Eic()
#g4e.source('/home/romanov/Downloads/beagle_eD.txt')
#g4e.run()

file = tempfile.NamedTemporaryFile(suffix='.mac')

file.write('')
print(file.name)
