Root Output
===========

G4E output data structure
-------------------------

- The g4e output is made out of so called aligned arrays.

- Each array holds values of a single entity e.g. **trk_mom** - hold momentums for each track in the event.

- Arrays are grouped by name prefixes, e.g  tracks (trk\_...), hits (hit\_...), generator particles (gen_prt\_...), etc.

- All arrays with the same name prefix have the same length. e.g. all trk\_... has the same length, the length is
  defined in trk_count variable (while can be retrieved with size() method - trk_mom.size())

- Arrays reference each other with id. This is close to relational DBs.
  In recent G4E versions (API v3) there is also *_index which reference directly an index in an array:

- All dimensions - mm. All energies and momentums - GeV

.. image:: _images/io_structure.svg
   :name: G4E output file structure



If event hierarchy is described like this:


.. code::

    event 1
        track 1 - electron 10GeV
            hit_11
            hit_12
            hit_13
        track 2 - proton   100GeV
            hit_21
            hit_22


In g4e output will be written like this:


.. code::

    event_id  = 1

    trk_count = 2
    trk_id    = [1,  2]
    trk_pdg   = [11, 2112]
    trk_mom   = [10, 100]
    trk_...   = [...    ]  - all arrays will be of 2 elements

    hit_count     = 5
    hit_...       = [...] - all arrays are of 5 elements
    hit_id        = [1,  2,   3,    4,  5]
    hit_trk_id    = [1,  1,   1,    2,  2] - link to tracks
    hit_trk_index = [0,  0,   0,    1,  1] - index in track array
    hit_z         = [10, 100, 200, -1, -5]


IDs vs Indexes
~~~~~~~~~~~~~~

(Only works with newer G4E)

There are two different fields **hit_trk_id** and **hit_trk_index**:

- hit_trk_id - represents a track id issued by Geant4 (unique inside an event). hit_trk_id-s
  may not correspond to the order of how they are written in trk\_... arrays and not all tracks are written

- hit_trk_index - points to track index in trk\_... array. So it is safe to use like this:
    .. code:: c++

       for(int i=0; i < hit_count; i++) {
          // Get parent track momentum
          double momentum = trk_mom[hit_trk_index[i]];
       }



Controlling what is written
---------------------------

saveSecondaryLevel
~~~~~~~~~~~~~~~~~~

To control how many generation of secondaries (tracks and their hits) to save, there is a configuration:

`/rootOutput/saveSecondaryLevel <ancestry-level>`

<ancestry-level> sets 0-n levels of ancestry which are saved in root file.

**Example:**

* -1 -  save everything
* 0  - save only primary particles
* 1  - save primaries and their daughters
* 2  - save primaries, daughters and daughters' daughters
* n  - save n generations of secondaries

(primaries - particles that came from a generator/input file)

The default level is 3, which corresponds to:

`/rootOutput/saveSecondaryLevel 3`

API v3
------

To check the version of api one can use TTree title (`events->GetTitle()`).
For API v3 it should be

.. code::

    > events = (TTree*) tfile->Get("events")
    > events->GetTitle()
   g4e_api_v3

Next are the tables with all values written to the tree:

Event
~~~~~

Event level information


========  =====================  ===============
  type      name                   description
========  =====================  ===============
uint64     event_id                Event number
double     evt_true_q2             Values of q2, x, etc. from generator
double     evt_true_x
double     evt_true_y
double     evt_true_w2
double     evt_true_nu
double     evt_true_t_hat
bool       evt_has_dis_info       If true - generator provided q2 and other event info
double     evt_weight             Generator provided event weight
========  =====================  ===============

**evt_has_dis_info** - Some generators (beagle) provide values like x, q2, etc. Some - don't this
flag indicates that this event has this information. If false any value evt_true\_* doesn't make sense


Hits
~~~~

========  =====================  ===============
  type      name                   description
========  =====================  ===============
uint       hit_count               Hits count
uint[]     hit_id                  Geant4 id
uint[]     hit_trk_id              Parent tack Geant4 id
uint[]     hit_trk_index           Index in parent track array
int[]      hit_pdg
string[]   hit_vol_name            Name of the volume
double[]   hit_x
double[]   hit_y
double[]   hit_z
double[]   hit_e_loss
uint[]     hit_type                0-sensitive, 1-vol. enter, 2-vol. leave
========  =====================  ===============

**hit_type** - indicates entity that saved the hit.

(defined in g4e::HitTypes structure)
- 0 - hit is saved from one of the sensitive detectors
- 1 - hit is saved because it is the first step in a volume
- 2 - hit is saved because it is the last step in a volume

Hits entering or exiting a volume are saved only for volumes marked in detector construction.


Tracks
~~~~~~

========  =====================  =============
  type      name                  description
========  =====================  =============
uint       trk_count              Tracks in events
uint[]     trk_id                 Geant4 id
uint[]     trk_parent_id          Parent track G4 id
uint[]     trk_parent_index       Parent track index in this array
int[]      trk_pdg                PDG code
uint[]     trk_create_proc        Geant4 code of created process
uint[]     trk_level              0 - generated prt, 1 - daughter, 2 - d
double[]   trk_vtx_x
double[]   trk_vtx_y
double[]   trk_vtx_z
double[]   trk_vtx_dir_x          Unit vector pointing track momentum at vtx
double[]   trk_vtx_dir_y
double[]   trk_vtx_dir_z
double[]   trk_mom                Track momentum
========  =====================  =============

Tracks are dynamic objects in Geant4 (changing on each step). Trk momentum
is snapshot when track is created, the momentum direction at this point is
stored at trk_vtx_dir\_[xyz]

*trk_level* - Is the level of ancestry from generated particle:
0  - generated particles, 1  - daughters of generated particles,
2 - daughters of daughters, etc.



Generated particles
~~~~~~~~~~~~~~~~~~~

Particle information from generator or particle gun whichever is used.

========  =====================  =============
  type      name                  description
========  =====================  =============
uint        gen_prt_count         Generated particles count
uint[]      gen_prt_id            Unique ID (from generator)
uint[]      gen_prt_vtx_id        ID of the origin vertex in vertex array
uint[]      gen_prt_vtx_index     Index of a vertex in vertex array
int[]       gen_prt_pdg           PDG code
uint[]      gen_prt_trk_id        Correspoinding track Geant4 ID
uint[]      gen_prt_trk_index     Corresponding track index in trk... arays
double[]    gen_prt_charge        Particle charge
double[]    gen_prt_dir_x         Unit vector pointing particle direction at origin
double[]    gen_prt_dir_y
double[]    gen_prt_dir_z
double[]    gen_prt_tot_mom       Total momentum
double[]    gen_prt_tot_e         Total energy
double[]    gen_prt_time
double[]    gen_prt_polariz_x
double[]    gen_prt_polariz_y
double[]    gen_prt_polariz_z
========  =====================  =============

Generated vertexes
~~~~~~~~~~~~~~~~~~

========  =====================  =============
  type      name                  description
========  =====================  =============
uint        gen_vtx_count         Generated vertexes count
uint[]      gen_vtx_id            Unique ID
uint[]      gen_vtx_part_count    Particles count
double[]    gen_vtx_x
double[]    gen_vtx_y
double[]    gen_vtx_z
double[]    gen_vtx_time
double[]    gen_vtx_weight
========  =====================  =============


CE Emcal
~~~~~~~~

Information of non empty central electron emcal cells

========  =====================  =============
  type      name                  description
========  =====================  =============
uint        ce_emcal_count        emcal non embpy modules count
string[]    ce_emcal_name         Volume name of the module
uint[]      ce_emcal_id           Module id (1000*row + col)
double[]    ce_emcal_etot_dep     Total energy deposit
uint[]      ce_emcal_npe
double[]    ce_emcal_adc
double[]    ce_emcal_tdc
double[]    ce_emcal_xcrs         X, Y Z of a cell center
double[]    ce_emcal_ycrs
double[]    ce_emcal_zcrs
uint[]      ce_emcal_row
uint[]      ce_emcal_col
uint[]      ce_emcal_section      0 PWO, 1 Glass
========  =====================  =============


