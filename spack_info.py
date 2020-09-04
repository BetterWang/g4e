import subprocess
import sys
import os
from subprocess import PIPE


this_g4e_home = os.path.dirname(os.path.abspath(__file__))


def get_location(packet_name):
    command = ['spack', 'location', '-i', packet_name]
    result = subprocess.run(command, encoding='utf8', stdout=PIPE, stderr=PIPE)

    if result.returncode:
        print("STDOUT: ", result.stdout)
        print("STDERR: ", result.stderr)        
        raise RuntimeError(f"Command {' '.join(command)} returned {result.returncode} code")

    return result.stdout.replace("\n","")

if __name__ == "__main__":
    print("CMake flags:\n")
    print("-DCMAKE_CXX_STANDARD=17")
    print(f"-DGEANT4_DIR={get_location('geant4')}")
    print(f"-DVGM_DIRECTORY={get_location('vgm')}")
    print(f"-DHEPMC_DIRECTORY={get_location('hepmc')}")
    print(f"-DCERN_ROOT_DIRECTORY={get_location('root')}")
    print("\nEnvironment vars:")
    print(f"G4E_HOME={this_g4e_home}")
    

    