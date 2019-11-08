MC Generators
=============


Selected generator is controlled with

`/generator/select`

possible values:

* ``particleGun`` - Particle gun
* ``hepmcAscii`` - HepMC 2 reader
* ``pythiaAscii`` - Lund (Pythia6) reader
* ``beagle`` - Beagle reader



BeAGLE format
-------------

BeAGLE - **B**enchmark **eA** **G**enerator for **LE**ptoproduction

BeAGLE, a Fortran program designed as a general purpose eA Monte-Carlo generator,
was based on DPMJetHybrid, but has been upgraded to improve various aspects.

`More info about BeAGLE <https://wiki.bnl.gov/eic/index.php/BeAGLE>`_

Control commands:

``/generator/beagle/verbose <level>`` - Set verbose level 0 = nothing, 1 = info, 2 = debug

``/generator/beagle/open <file name>`` - Opens a beagle event file to read



HEPMC2 format
-------------

The HepMC package is an object oriented event record for Monte Carlo Generators.

HepMC2 which is used by Pythia8 or Herwig.


http://lcgapp.cern.ch/project/simu/HepMC/

Control commands:

``/generator/hepmcAscii/verbose <level>`` - Set verbose level 0 = nothing, 1 = info, 2 = debug

``/generator/hepmcAscii/open <file name>`` - Opens a beagle event file to read



LUND format
-----------

Ascii format from Pythia6 and other

`BNL wiki on Pythia <https://wiki.bnl.gov/eic/index.php/PYTHIA>`_

Control commands:

``/generator/pythiaAscii/verbose <level>`` - Set verbose level 0 = nothing, 1 = info, 2 = debug

``/generator/pythiaAscii/open <file name>`` - Opens a beagle event file to read



Sample configuration
--------------------

.. code:: yaml

    #------------------------------------------------------------------------
    # Macro file for opening MC files
    #------------------------------------------------------------------------
    #

    # Include default setup
    # Use jleic.mac for headless mode or jleicvis.mac for GUI visualization
    /control/execute jleic.mac

    # Select Beagle generator
    /generator/select beagle

    # Open file and run some MC file
    /generator/beagle/open beagle_test_data.txt
    /generator/beagle/verbose 2

    /run/beamOn 1
    exit


See also :doc:`Howto process MC files <howto_process_mc_files>`