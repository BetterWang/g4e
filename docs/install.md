# Installation

## Use ejpm

ejpm is EIC software centric package/build manager. It helps building
EIC software stack with some its dependencies and later manage the packages

First, install ejpm itself:

```bash
pip install --user ejpm
```
(If you have certificate problems (JLab issue), don't have pip, or have other problems, 
[here is the detailed documentation](https://gitlab.com/eic/ejpm) )

Install g4e and possible other EIC packets (ejana, eic-smear, etc.)
```bash
# 1. System prerequesties
ejpm req centos g4e      # get list of required OS packets. Use `ubuntu` on debian  
sudo yum install ...     # install watever 'ejpm req' tells you

# 2. Where to install
ejpm --top-dir=<where-to>   # Directory where packets will be installed

# 3. Install
ejpm install g4e            # install 'Geant 4 EIC' and dependencies (like vgm, hepmc)

# 4.  Source environment
source ~/.local/share/ejpm/env.sh  # Use *.csh file for tcsh
```

You have ROOT and Geant4 and don't want EJPM to build them?  
(Use your installations of ROOT and Geant4)

```
# Before running 'ejpm install g4e'
ejpm set root `$ROOTSYS`    # Path to ROOT installation
ejpm set geant <path>       # Path to Geant4 installation   
```

Hint (!). Run ejpm to overview all installed packets, environment and status by 'ejpm' command

Here is the sample output:  
```
> ejpm

EJPM v0.01.19
top dir :
    /eic
state db :
    ~/.local/share/ejpm/db.json  (users are encouraged to inspect/edit it)
env files :
    ~/.local/share/ejpm/env.sh
    ~/.local/share/ejpm/env.csh

INSTALLED PACKETS: (*-active):
 vgm:
    * /eic/vgm/vgm-v4-5 (owned) 
 root:
    * /eic/root/root-v6-16-00 (owned)
 geant:
      /eic/geant/geant-v10.5.0 (owned)
    * /eic/geant4-10.6-betta
 hepmc:
    * /eic/hepmc/hepmc-HEPMC_02_06_09 (owned) 
 g4e:
    * /eic/g4e/g4e-dev (owned)
```



## CMake build

```bash
git clone https://gitlab.com/jlab-eic/g4e.git
mkdir g4e/build
cd g4e/build
cmake ../
make
```

### Dependencies

- **Geant4**
   - CMake flag: `GEANT4_DIR`   
- **VGM**  
   Provides export to CERN.ROOT geometry
   - Env. variable: `VGM_DIRECTORY` or `VGM_DIR`
   - CMake flag: `VGM_DIRECTORY`
- **HEPMC2** (optional)  
    Required to open Pythia8 and other HepMC2 compatible MC files   
   - Env. variable: `HEPMC_DIR` or `HEPMC_ROOT_DIR`
   - CMake flag: `HEPMC_DIRECTORY` or `HEPMC_DIR`
- **Cern ROOT**  
   Required for IO and some histogramming
   - Environment variable: `ROOTSYS`
   - CMake flag: `CERN_ROOT_DIRECTORY`


### CMake flags

Additional flags:
`WITH_GEANT4_UIVIS` = ON Build with Geant4 UI and Vis drivers. Default
`CMAKE_CXX_STANDARD` = 11 - C++ standard used.





