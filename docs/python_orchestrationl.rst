Python Orchestration
====================

G4E provides Python orchestration library 'g4epy' which is a thin wrapper around g4e executable.

**g4epy** allows to configure G4E run in python. When **run()** method is executed, the appropriate
Geant4 macro file is created and **g4e** executable is run with the right flags.

Example configuration

.. code:: python

    from g4epy import Geant4Eic

    g4e = Geant4Eic(beamline='ip6')

    # Set output file names. Extension is omitted here as g4e creates
    # several files with this name and different extensions
    g4e.output('work/g4e_emcal')

    # (!) g4e.py will automatically identify the file format
    g4e.source('work/beagle.txt') \

    g4e.beam_on(1000)   # Process 1000 events
    g4e.run()           # Runs G4E with the above configuration

Functions can be stacked. The same configuration can be written as:

.. code:: python

    from g4epy import Geant4Eic

    g4e = Geant4Eic(beamline='ip6')

    g4e.source('work/beagle.txt') \
       .output('work/g4e_emcal') \
       .beam_on(1000) \
       .run()

**vis()** function can be added to run the same configuration with Geant4 GUI event/geometry viewer

.. code:: python

    from g4epy import Geant4Eic

    g4e = Geant4Eic(beamline='ip6')

    # For bare geometry display we don't need source and output
    g4e.beam_on(0)
    g4e.vis()        # <== enable Geant4 GUI
    g4e.run()

**command()** function allows to add any custom Geant4 macro commands. Might be a string or a list

.. code:: python

    # Set beam energies (use list)
    g4e.command(['/eic/refdet/eBeam 5',
                 '/eic/refdet/pBeam 50'
                 ])

    # Single string command
    g4e.command('/eic/detector refdet')


Geant4Eic class
---------------

Wrapper class responsible for one instance of running G4E

Functions:

- **__init__(\*\*kwargs)**
  Parameters:

  - **executable** : Full path to g4e executable (if not set env variable 'G4E_HOME' is used to auto determine g4e path)
  - **build_prefix** : see 'Building g4e with python`
  - **beamline** : Beamline name ip6 or ip8
  - **detector** : Main detector construction name. Default 'refdet'
  - **is_batch** : True - no GUI, False - Geant4 event display
  - **thread_count** : Number of Threads (parallel workers)

- **command(value)**->self - executes Geant4 macro command/s

  if value:

  - string - execute string
  - list of strings - execute all strings
  - list of list of strings - each sublist is " ".join and executed as list of strings

- **source(source_file)**->self - sets source file names. Might be a list
- **vis(run_vis=True)**->self - Sets visualization display Geant4 GUI
- **batch(run_batch=True)**->self - Sets if Geant runs in batch (no visualization) mode
- **threads(thread_count)**->self - Sets the number of working threads
- **get_run_command()**->self - Returns the command that will be used to run g4e
- **beam_on(events_number)**->self - Sets the number of events to process
- **output(base_name)**->self - Sets base name of output files (G4E produces a number of files with different extensions)
- **run()**->retval, start_time, end_time, output_lines - Runs g4e


Geant4EicManager class
----------------------

G4E might be rebuilt right from the python code. This is sometimes convenient when working with Jupyter notebooks
Geant4EicManager allows to do this


.. code:: python

    from g4epy import Geant4EicManager

    manager = Geant4EicManager()
    manager.build()

    # Most of the time man.build() is the only one needs
    # but configure and clean are also available:
    #   manager.cmake_configure()
    #   manager.clean()