
from g4epy import Geant4EicManager

man = Geant4EicManager()

print(man)

man.configure()
man.clean()
man.build()
