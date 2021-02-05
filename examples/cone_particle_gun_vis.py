from g4epy import Geant4Eic

g4e = Geant4Eic(beamline='erhic')

# Select cone particle gun generator
g4e.command('/generator/select coneParticleGun')

# Set particle to be generated.
# (geantino is default)
# (ion can be specified for shooting ions)
# Common names: proton, e- e+ pi+ pi-
#g4e.command('/generator/coneParticleGun/particle e-')
g4e.command('/generator/coneParticleGun/particle proton')

# Set momentum direction
# Direction needs not to be a unit vector
#g4e.command('/generator/coneParticleGun/direction 0 0 -1')
g4e.command('/generator/coneParticleGun/direction 0.249973959 0 0.996875163')

# Set kinetic energy [GeV]
g4e.command('/generator/coneParticleGun/energy 275 GeV')

# Energy spread [GeV],
# energy is smeared as gauss (mean=<particle E>, stddev=energyStdDev)
g4e.command('/generator/coneParticleGun/energyStdDev 1 GeV')

# Cone angle standard deviation.
# Basically is the resulting cone angle
g4e.command('/generator/coneParticleGun/coneAngleStdDev 0.3 rad')

# Set number of particles to be generated
g4e.command('/generator/coneParticleGun/number 1')

# To control how many generation of secondaries (tracks and their hits) to save,
# there is a configuration:
#    /rootOutput/saveSecondaryLevel <ancestry-level>
#
# <ancestry-level> sets 0-n levels of ancestry which are saved in root file.
#
# Example:
#
# -1 - save everything
# 0 - save only primary particles
# 1 - save primaries and their daughters
# 2 - save primaries, daughters and daughters’ daughters
# n - save n generations of secondaries
#
# (primaries - particles that came from a generator/input file)
#
# The default level is 3, which corresponds to:
#
# /rootOutput/saveSecondaryLevel 3
#
# We set it to 1. If only vertex particles are of the interest, set it to 0
#
# This flag doesn't affect physics in g4e (only what is saved)
# so EM showers in EMCAL is fully simulated with any value here
g4e.command(['/rootOutput/saveSecondaryLevel 0'])

# Extension is omitted here
# g4e creates a bunch of files with this name and different extensions
g4e.output('cone_particle_gun')

# Run g4e run!!!
#g4e.beam_on(300).run()

# Event display config
g4e.command(['/vis/viewer/addCutawayPlane 0 0 7 m 0 0 1',
             '/vis/viewer/addCutawayPlane 0 0 7 m 1 0 0',
             '/vis/viewer/addCutawayPlane 0 0 7 m 0 -1 0',
             '/vis/viewer/set/style surface',
             '/vis/viewer/colourByDensity 1 g/cm3 .5 3 10',
             '/vis/viewer/set/background 1 1 1'])
g4e.command(['/vis/viewer/set/viewpointThetaPhi 90 135'])

# There are a lot of particles
g4e.command('/vis/ogl/set/displayListLimit 150000')

# Run with event display of 10 events
g4e.beam_on(10).vis().run()
