from g4epy import Geant4EicManager

man = Geant4EicManager()

print(man)

man.cmake_configure()
man.clean()
man.build()
