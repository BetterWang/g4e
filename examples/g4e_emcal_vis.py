import os
from g4epy import Geant4Eic

g4e = Geant4Eic(beamline='ip6')


# If one would like to open a file
# What is the source
#g4e.source('work/eC_10x41_GCF_QE_Egaus_small.txt')

# Setup particle gun
# Please see particle gun example for the full explanation
g4e.command('/generator/select coneParticleGun')                    # Select cone particle gun generator
g4e.command('/generator/coneParticleGun/particle e-')               # Fire electrons
g4e.command('/generator/coneParticleGun/direction 0 0 -1')          # Direction to EMCAL
g4e.command('/generator/coneParticleGun/energy 6 GeV')              # Set kinetic energy [GeV]
g4e.command('/generator/coneParticleGun/energyStdDev 1 GeV')        # Energy spread [GeV],
g4e.command('/generator/coneParticleGun/coneAngleStdDev 0.4 rad')   # Cone angle
g4e.command('/generator/coneParticleGun/number 10')                 # N particles to be generated per event


# What is the output
# g4e creates a bunch of files with this name and different extensions
g4e.output('vis')

# Next we will switch things on and off
# /vis/geometry/set/visibility has 3 parameters:
# /vis/geometry/set/visibility <logic name> <depth> <on/off>
# If depth -1 - all children are affected

# Turn off everything in the world
g4e.command('/vis/geometry/set/visibility World_Logic -1 0')

# Turn on only those we need:

# g4e.command([
#      '/vis/geometry/set/visibility ffi_OFFM_TRK_GVol_Logic    -1  1',
#      '/vis/geometry/set/visibility ffi_OFFM_TRK_lay_Logic     -1  1',
#      '/vis/geometry/set/visibility ffi_OFFM_TRK_lay_BH_Logic  -1  1',
#      '/vis/geometry/set/visibility ffi_OFFM_TRK_lay_BH_Logic  -1  1',
#      '/vis/geometry/set/visibility ffi_OFFM_TRK2_GVol_Logic   -1  1',
#      '/vis/geometry/set/visibility ffi_OFFM_TRK2_lay_Logic    -1  1',
#      '/vis/geometry/set/visibility ffi_NEG_TRK_GVol_Logic     -1  1',
#      '/vis/geometry/set/visibility ffi_NEG_TRK_lay_Logic      -1  1',
#      '/vis/geometry/set/visibility ffi_ZDC_GVol_Logic         -1  1',
#      '/vis/geometry/set/visibility ffi_RPOT_D2_GVol_Logic     -1  1',
#      '/vis/geometry/set/visibility ffi_RPOT_D2_lay_Logic_0    -1  1',
#      '/vis/geometry/set/visibility ffi_RPOT_D2_lay_Logic_1    -1  1',
#      '/vis/geometry/set/visibility ffi_RPOT_D2_lay_Logic_2    -1  1',
#      '/vis/geometry/set/visibility ffi_RPOT_D2_lay_Logic_3    -1  1',
#      '/vis/geometry/set/visibility ffe_LOWQ2_GVol_Logic       -1  1',
#      '/vis/geometry/set/visibility ffe_BPC_Logic              -1  1',
#      '/vis/geometry/set/visibility ffe_LOWQ2_lay_Logic        -1  1',
#              ])


g4e.command([
    '/vis/geometry/set/visibility cb_Solenoid_GVol_Logic  0  1',
    '/vis/geometry/set/visibility cb_EMCAL_GVol_Logic     0  1',
    '/vis/geometry/set/visibility cb_HCAL_GVol_Logic      0  1',
    '/vis/geometry/set/visibility ci_ENDCAP_GVol_Logic    0  1',
    '/vis/geometry/set/visibility cb_VTX_GVol_Logic      -1  1',
    '/vis/geometry/set/visibility ce_GEM_GVol_Logic      -1  1',
    '/vis/geometry/set/visibility ce_MRICH_GVol_Logic    -1  1'
])

# Make them transparent
# ! - means omit parameter (those are RGB)
g4e.command([
    '/vis/geometry/set/colour cb_EMCAL_GVol_Logic        0 ! ! ! 0.1',
    '/vis/geometry/set/colour cb_HCAL_GVol_Logic         0 ! ! ! 0.1',
    '/vis/geometry/set/colour ci_ENDCAP_GVol_Logic       0 ! ! ! 0.1',
    '/vis/geometry/set/colour ce_GEM_GVol_Logic         -1 ! ! ! 0.1',  # -1 - all sub elements
    '/vis/geometry/set/colour ce_MRICH_GVol_Logic       -1 ! ! ! 0.1',

    '/vis/geometry/set/forceAuxEdgeVisible cb_EMCAL_GVol_Logic     0  1',

])


g4e.command([
    '/vis/geometry/set/visibility ce_EMCAL_GVol_Logic      -1 1',
    '/vis/geometry/set/visibility ce_EMCAL_detPWO_Logic    -1 1',
    '/vis/geometry/set/visibility ce_EMCAL_detGLASS_Logic  -1 1',
])


# We want to clear default cutaway plane:
# g4e.command('/vis/viewer/set/cutawayMode intersection')
g4e.command('/vis/viewer/clearCutawayPlanes')
g4e.command('/vis/viewer/clearCutawayPlanes')

# Just in case the default one is:
# g4e.command('/vis/viewer/addCutawayPlane 0  30  0  cm  0. -500 0')
# XY plane
# g4e.command('/vis/viewer/addCutawayPlane  1  0  0  cm   0  0  -0.001')

# There are a lot of particles
g4e.command('/vis/ogl/set/displayListLimit 150000')

g4e.vis()   # - SHOW EVENT DISPLAY (use less number of events)

# Number of events to process
g4e.beam_on(2)

# This starts the simulation
g4e.run()

