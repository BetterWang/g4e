#include "MainRootOutput.hh"

#include <TFile.h>

g4e::MainRootOutput::MainRootOutput(TFile *rootFile):
    mRootFile(rootFile),
    jleicRootOutput(new JLeicRootOutput()),
    jleicHistos(new JLeicHistogramManager())
{
    // We also create JLeic root output here , while g4e is in transition
    // TODO Move JLeic initialization to the appropriate phase
    jleicRootOutput->Initialize(mRootFile);
}



