
target_include_directories(g4e PUBLIC
        generic_lund_reader
        beagle_reader
        hepmc2_reader
        pythia_ascii_reader
        cone_particle_gun
        )

target_sources_local(g4e
        PRIVATE
            # Primary user action and a messenger
            PrimaryGeneratorAction.cc
            PrimaryGeneratorAction.hh
            PrimaryGeneratorMessenger.cc
            PrimaryGeneratorMessenger.hh

            # Generic lund reader
            generic_lund_reader/TextEventFileReader.hh      # Include here for IDE support

            # Beagle event generator reader
            beagle_reader/BeagleReader.cc
            beagle_reader/BeagleReader.hh
            beagle_reader/BeagleGeneratorMessenger.cc
            beagle_reader/BeagleGeneratorMessenger.hh
            beagle_reader/BeagleGenerator.cc
            beagle_reader/BeagleGenerator.hh

            # HepMC2 event generator reader
            hepmc2_reader/HepMcAsciiGenerator.cc
            hepmc2_reader/HepMcAsciiGenerator.hh
            hepmc2_reader/HepMcAsciiReaderMessenger.cc
            hepmc2_reader/HepMcAsciiReaderMessenger.hh
            hepmc2_reader/HepMcGeneratorInterface.cc
            hepmc2_reader/HepMcGeneratorInterface.hh

            # Pythia 6 lund format reader
            pythia_ascii_reader/PythiaAsciiReaderMessenger.hh
            pythia_ascii_reader/PythiaAsciiReader.cc
            pythia_ascii_reader/PythiaAsciiReader.hh
            pythia_ascii_reader/PythiaAsciiReaderMessenger.cc

            cone_particle_gun/ConeParticleGun.cc
            cone_particle_gun/ConeParticleGun.hh
            cone_particle_gun/ConeParticleGunMessenger.cc
            cone_particle_gun/ConeParticleGunMessenger.hh

            #cone_generator/ConeGenerator.cc
            #cone_generator/ConeGenerator.hh
        )