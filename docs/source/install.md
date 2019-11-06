# Installation

## Use ejpm

First, install ejpm itself:
```
pip install --user ejpm
```
(If you have certificate problems (JLab issue), don't have pip, or have other problems, 
[here is the detailed documentation](https://gitlab.com/eic/ejpm) )

Install g4e and possible other EIC packets (ejana, eic-smear, etc.)
```bash
# 1. System prerequesties
ejpm req centos g4e      # get list of required OS packets. Use `ubuntu` on debian  
sudo yum install ...     # install watever 'ejpm req' shows

# 2. Where to install
ejpm --top-dir=<where-to>   # Directory where packets will be installed

# 3. Install
ejpm install g4e            # install 'Geant 4 EIC' and dependencies (like vgm, hepmc)

# 4.  Source environment
source ~/.local/share/ejpm/env.sh  # Use *.csh file for tcsh
```

You have ROOT and Geant4 and don't want EJPM to build them?  
(Use your ROOT and Geant)

```
# Before running ejpm install g4e
ejpm set root `$ROOTSYS`    # (optional) if you have CERN.ROOT or other monster packets:
ejpm set geant <path>       # Path to your geant4 installation   
```

Hint (!). Run ejpm to overview all isntalled packets, environment and status by 'ejpm' command

```
> ejpm
```

# 



## Build from CMake

```bash
git clone https://gitlab.com/jlab-eic/g4e.git
mkdir g4e/build
cd g4e/build
cmake ../
make
```

### Dependencies

- Geant4
   - CMake flag: `GEANT4_DIR`   
- VGM
   - Env. variable: `VGM_DIRECTORY` or `VGM_DIR`
   - CMake flag: `VGM_DIRECTORY`
- HEPMC (2)
   - Env. variable: `HEPMC_DIR` or `HEPMC_ROOT_DIR`
   - CMake flag: `HEPMC_DIRECTORY` or `HEPMC_DIR`
- Cern ROOT
   - Environment variable: `ROOTSYS`
   - CMake flag: `CERN_ROOT_DIRECTORY`


### Flags

Additional flags:
`WITH_GEANT4_UIVIS` = ON Build with Geant4 UI and Vis drivers. Default
`CMAKE_CXX_STANDARD` = 11 - C++ standard used.

### Environment

After installation, when running, g4e looks for:

```G4E_HOME``` - which should point to a place, where 'resources' 
folder is located

```G4E_MACRO_PATH``` - allows to add aditional paths where *.mac files 
are checked. The rule is the same as system PATH (colon separated list of pahts) 

