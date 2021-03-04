Visualization
=============

There are 2 ways to view the visualization of the geometry:

1. On each run G4E creates \<output_name\>.geo.root file with the constructed geometry.
   You can see this geometry through root tools (which you probably did in jupyeter),
   and this geometry is used for reconstruction part.

2. One can run regular Geant4 event display.
   For this part, if you are running docker you have to attach x11,
   please see the bottom of this readme file of how to do this with docker https://gitlab.com/eic/containers
   You have to run g4e with --gui flag and then all standard geant4 macro will work.
   G4E provides jleicvis.mac general macro with the visualization setup. Here is an example, which runs geant4 visualization:

Example:

Macro file:
.. code::

   /control/execute refdet.vis.mac
   /eic/beamline/name ip6
   /generator/select beagle
   /generator/beagle/open /home/romanov/eic/data/eC_10x110_GCF_QE_Egaus_noINC_withcross.txt
   /run/initialize
   /run/beamOn 10

Execute it:

   g4e --gui <macro_file_name>
