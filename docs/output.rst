Output
======



Controlling secondaries
-----------------------

To control how many secondaries (tracks and their hits) to save, there is a configuration:

`/rootOutput/saveSecondaryLevel <ancestry-level>`

<ancestry-level> sets 0-n levels of ancestry which are saved in root file.

**Example:**

* -1 -  save everything
* 0  - save only primary particles
* 1  - save primaries and their daughters
* 2  - save primaries, daughters and their daughters

(primaries - particles that came from a generator/input file)

The default level is 3, which corresponds to:

`/rootOutput/saveSecondaryLevel 3`
