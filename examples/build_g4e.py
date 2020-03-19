# G4E might be rebuilt right from the python code


from g4epy import Geant4EicManager

manager = Geant4EicManager()
manager.build()

# Most of the time man.build() is the only one needs
# but configure and clean are also available:
#   manager.cmake_configure()
#   manager.clean()

