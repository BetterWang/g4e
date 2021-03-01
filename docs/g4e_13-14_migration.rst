v1.3.x to 1.4.x migration
=========================

G4E v1.4 brought several breaking changes: 


1. The major change in terms of users macros and python scripts is that lots of macro commands were changed and systematyzed: 


========================  ==================
 1.3                       1.4
========================  ==================
/g4e/*                    /eic/*
/detsetup/                /eic/refdet/*
/detsetup/beamlineName    /eic/beamline/name
/XTRdetector              /eic/ci_TRD
/emphyslist               /eic/ci_TRD
/generator/               Not changed!
========================  ==================

Example: 

.. code:: yaml

   /detsetup/eBeam 10      # Old
   /detsetup/pBeam 110     # Old

   /eic/refdet/eBeam 10    # New!
   /eic/refdet/pBeam 110   # New!  


2. JLeic detector was renamed to EIC reference detector. Main macro files changed to `refdet`

========================  ==================
 1.3                      1.4
========================  ==================
jleic.mac                 refdet.mac
jleicvis.mac              refdet.vis.mac
========================  ==================

If previously many macro files started with executing jleic.mac or jleicvis.mac now they need to be changed to refdet.mac and refdet.vis.mac.

.. code:: yaml

   # Beginning of file 
   /control/execute jleic.mac    # Old (!) will not work (!)

   # Use refdet.mac for headless mode or refdet.vis.mac for GUI visualization
   /control/execute refdet.mac   # New


Look at `examples macro and python scripts`_

3. JLeic* classes where renamed to ReferenceDetector* classes. For example JLeicDetectorConstruction now is named ReferenceDetectorConstruction. Look at `main_detectors`_

4. All sensitive are now created in ConstructSDandField() according to Geant4 multithreading model. 

5. Output data format has several new fields. The format is backward compatible (no previous fields changed or removed). Look at `output`_ section


.. _examples macro and python scripts: https://gitlab.com/eic/escalate/g4e/-/tree/master/examples
.. _main_detectors: https://gitlab.com/eic/escalate/g4e/-/tree/master/src/main_detectors
