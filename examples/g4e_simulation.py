from g4epy import Geant4Eic

g4e = Geant4Eic(beamline='ip6')

# Beagle provides beam energy information, but for the sake of simplicity we
g4e.command(['/eic/refdet/eBeam 5',
             '/eic/refdet/pBeam 50'
             ])

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
g4e.command(['/rootOutput/saveSecondaryLevel 1'])

# Extension is omitted here
# g4e creates a bunch of files with this name and different extensions
g4e.output('work/g4e_emcal')

# Run g4e run!!!
g4e.source('work/beagle.txt') \
   .beam_on(1000) \
   .run()