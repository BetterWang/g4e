Increase simulation speed
=========================


Controlling secondary level
---------------------------

Sometimes one needs only propagate generated particles (no daughters) or n-th level of particle decays. Example:

.. code::

    Ancestry level
     0         1       1         2      2
    Sigma -> gamma + lambda -> proton + pi-
     ^
     |
     +--- Sigma came from generator or part. gun

One can control what particles are killed in g4e tracking action (for example kill everything after 2nd ancestry level)
and what particles are saved into root files.

.. code::

   /eic/tracking/saveSecondaryLevel   <ancestry-level>
   /eic/rootOutput/saveSecondaryLevel <ancestry-level>


There is a drastic difference between the two:

- **/eic/tracking/saveSecondaryLevel** kills tracks during
  tracking action, so no e.g. calorimeter showers and things like this are being fully simulated. But such
  cutting significantly increases the performance (sometimes by orders of magnitude).

- **/eic/rootOutput/saveSecondaryLevel** only controls what is saved to root file and only affects
  **hits\_** and **trk\_** fields. So e.g. all calorimeter information is still simulated and saved but the file size is reduced
  because less bare hits and unnecessary tracks are being saved. It also affects the performance but in terms of IO and
  writing large files.

Both flags works the same in terms of what levels mean:

* -1 -  save everything
* 0  - save only primary particles
* 1  - save primaries and their daughters
* 2  - save primaries, daughters and daughters' daughters
* n  - save n generations of secondaries

(primaries - particles that came from a generator/input file)

The default level for ROOT IO is 3, which corresponds to:

`/eic/rootOutput/saveSecondaryLevel 3`