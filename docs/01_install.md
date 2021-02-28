# Build/Install

There are several ways how one can install packages locally:

1. Spack package manager
2. EJPM package manager
3. Combined Conda+EJPM isntallation
4. Manual build

What packet manager to use? **The recommendation is**: to use EJPM for personal laptops with Linux OS for personal use and development. Use Spack on farms or if you are familiar with Spack or use Spack for other projects.

Read more here: "[Detailed comparison between EJPM and Spack](https://escalate.readthedocs.io/en/latest/204_ejpm_vs_spack_detailed.html#spack-ejpm-comparison)"

## Use ejpm

> EJPM is still available and mantained but further development is
> concentrated around Spack packet manager

ejpm is EIC software centric package/build manager. It is the designed
to be the default tool to build G4E on users machine, as it helps with:

- building dependent packages (with "right" compilation flags)
- setup environment variables to run everything
- what packages to install by system packet manager
- rebuild/update/remove/clean existing packages

> Still, ejpm is not a requirement.

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

# 3. Config: it is suggested to use C++17 if compiler allows it
ejpm config global cxx_standard=17 

# 4. Install
ejpm install g4e            # install 'Geant 4 EIC' and dependencies (like vgm, hepmc)

# 5.  Source environment
source ~/.local/share/ejpm/env.sh  # Use *.csh file for tcsh
```

You have ROOT and Geant4 and don't want EJPM to build them?  
(Use your installations of ROOT and Geant4)

```
# Before running 'ejpm install g4e'
ejpm set root `$ROOTSYS`    # Path to ROOT installation
ejpm set geant <path>       # Path to Geant4 installation   
```

> (!) If you use your version of ROOT, all packages depending on ROOT should be
> installed with the same C++ standard flags as root. So it it was C++11 or C++17, it should be used
> everywhere. To set it in ejpm  
> ```ejpm config global cxx_standard=17```
>

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


## Spack install
Spack is a package management tool designed to support multiple versions and configurations of 
software on a wide variety of platforms and environments. Spack allows to automatically build
target packages with all needed dependencies. [Sapck documentation](https://spack.readthedocs.io/en/latest/getting_started.html#installation)

The installation consist of 3 steps then:
1. Install spack itself
2. Install EIC [repository](https://github.com/eic/eic-spack) (with EIC packages)
3. Run spack command to install g4e (or other packages)


### Install spack and EIC repository

```bash 
git clone https://github.com/spack/spack.git

#Source environment

# For bash/zsh users
$ . spack/share/spack/setup-env.sh

# For tcsh/csh users
$ source spack/share/spack/setup-env.csh
```

You should be able now to use spack:

```bash
spack info root
```

> (!) By default, all packages will be downloaded, built and installed in this spack directory
>
[More documentation on spack installation](https://spack.readthedocs.io/en/latest/getting_started.html#installation)


Clone and add [eic-spack repository](https://github.com/eic/eic-spack):

```bash
# Adding the EIC Spack Repository
git clone https://github.com/eic/eic-spack.git

# Add this repository to your Spack configuration
spack repo add eic-spack
```


### Install and use g4e with spack

To install g4e with spack (this will install the latest stable version)

```bash
spack install g4e
```

To install concrete version of spack:

```bash
spack info g4e            # to see the available versions
spack install g4e@1.3.7   # 
```

To see what is going to be installed/built

```bash
spack speck -I g4e
```

To use G4E with spack:

```bash
spack load g4e

# or with exact version
spack load g4e@1.3.7
```

[More documentation of spack usage](https://spack.readthedocs.io/en/latest/basic_usage.html)

<br>

### Develop with spack

The simplest way to have g4e development version where everything
is installed with spack is to load g4e installed by Spack and then
redirect environment variables to your development directory
```bash
spack install g4e@master
spack load g4e@master
export G4E_HOME=/your/g4e/development/directory
mkdir build &&cd build
cmake -DCMAKE_CXX_STANDARD=17 $G4E_HOME
make
```


## CMake build

```bash
git clone https://gitlab.com/eic/escalate/g4e.git
mkdir g4e/build
cd g4e/build
cmake -DGEANT4_DIR=<geant4 installation> ../
make
```

### Dependencies

- **Geant4**
   - CMake flag: `GEANT4_DIR`   
- **VGM**  
   Provides export to CERN.ROOT geometry
   - Env. variable: `VGM_DIRECTORY` or `VGM_DIR`
   - CMake flag: `VGM_DIRECTORY`
- **HEPMC2**
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
