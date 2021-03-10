from g4epy import Geant4Eic

g4e = Geant4Eic(beamline='ip6')

# Select cone particle gun generator
g4e.command('/generator/select coneParticleGun')

# Set particle to be generated.
# (geantino is default)
# (ion can be specified for shooting ions)
# Common names: proton, e- e+ pi+ pi-
#g4e.command('/generator/coneParticleGun/particle e-')
g4e.command('/generator/coneParticleGun/particle neutron')

# Set momentum direction
# Direction needs not to be a unit vector
#g4e.command('/generator/coneParticleGun/direction 0 0 -1')
g4e.command('/generator/coneParticleGun/direction 0 0 0')

# Set kinetic energy [GeV]
g4e.command('/generator/coneParticleGun/energy 275 GeV')

# Energy spread [GeV],
# energy is smeared as gauss (mean=<particle E>, stddev=energyStdDev)
g4e.command('/generator/coneParticleGun/energyStdDev 1 GeV')

# Cone angle standard deviation.
# Basically is the resulting cone angle
g4e.command('/generator/coneParticleGun/coneAngleStdDev 0.25 rad')

# Set number of particles to be generated
g4e.command('/generator/coneParticleGun/number 5')

# To control how many generation of secondaries (tracks and their hits) to save, 0 - track only generated particles
g4e.command(['/eic/tracking/saveSecondaryLevel 0'])

# Extension is omitted here
# g4e creates a bunch of files with this name and different extensions
g4e.output('neutron_particle_gun')

# Enabling visualization
# (!) It should be done prior to /vis/... commands
g4e.vis()

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
g4e.beam_on(10).run()
