Generators
==========

Selected generator is controlled with

`/generator/select`

possible values:

* ``coneParticleGun`` - Particle with gaussian smeared cone
* ``particleGun`` - Standard Geant4 particle gun
* ``hepmcAscii`` - HepMC 2 reader
* ``pythiaAscii`` - Lund (Pythia6) reader
* ``beagle`` - Beagle reader


Cone particle gun
-----------------

`/generator/coneParticleGun/`

:doc:`Example macro for coneParticleGun <example_cone_particle_gun>`



BeAGLE format
-------------

BeAGLE - Benchmark eA Generator for LEptoproduction

BeAGLE, a Fortran program designed as a general purpose eA Monte-Carlo generator,
was based on DPMJetHybrid, but has been upgraded to improve various aspects.

`More info about BeAGLE <https://wiki.bnl.gov/eic/index.php/BeAGLE>`_

Control commands:

``/generator/beagle/verbose <level>`` - Set verbose level 0 = nothing, 1 = info, 2 = debug

``/generator/beagle/open <file name>`` - Opens a beagle event file to read

`Example of BeAGLE file (3 events) <https://gitlab.com/jlab-eic/g4e/raw/master/test/beagle/beagle_test_data.txt>`_



HEPMC2 format
-------------

The HepMC package is an object oriented event record for Monte Carlo Generators.

HepMC2 which is used by Pythia8 or Herwig.


http://lcgapp.cern.ch/project/simu/HepMC/

Control commands:

``/generator/hepmcAscii/verbose <level>`` - Set verbose level 0 = nothing, 1 = info, 2 = debug

``/generator/hepmcAscii/open <file name>`` - Opens a HepMC2 event file to read



LUND format
-----------

Ascii format from Pythia6 and other

`BNL wiki on Pythia <https://wiki.bnl.gov/eic/index.php/PYTHIA>`_

Control commands:

``/generator/pythiaAscii/verbose <level>`` - Set verbose level 0 = nothing, 1 = info, 2 = debug

``/generator/pythiaAscii/open <file name>`` - Opens a Pythia6 event file to read




Sample configuration
--------------------

.. include:: example_process_mc.rst


See also

:doc:`Howto process MC files <howto_process_mc_files>`

