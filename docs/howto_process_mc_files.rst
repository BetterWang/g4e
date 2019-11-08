Process MC files
================

Quick guide of how to run process MC files with g4e.


1. Create ``run_mc.mac`` file with contents:

.. include:: example_process_mc.rst


2. Download BeAGLE sample file from here:

https://gitlab.com/eic/epw/raw/master/data/beagle_eD.txt


3. Run g4e like:

.. code:: bash

   g4e run_mc.mac


As the result at least 3 files should be created:

* ``jleicGeometry.root`` - A file with the root geometry
* ``jleicGeometry.gdml`` - Gdml version of the geometry
* ``g4e_out.root``` - events


4. To open visualization GUI change ``run_mc.mac``. Use ``jleicvis.mac`` instead of ``jleic.mac``
