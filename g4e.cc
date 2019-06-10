//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"

#include "JLeicDetectorConstruction.hh"
#include "JLeicPhysicsList.hh"
#include "JLeicPrimaryGeneratorAction.hh"
#include "JLeicRunAction.hh"
#include "JLeicEventAction.hh"
#include "JLeicSteppingAction.hh"
#include "JLeicSteppingVerbose.hh"
#include "JLeicTrackingAction.hh"

#ifdef G4VIS_USE

#include "G4VisExecutive.hh"

#endif

#ifdef G4UI_USE

#include "G4UIExecutive.hh"

#endif

//-- physics processes --
#include "FTFP_BERT.hh"

#include "clara.hh"

/// Program Configuration provided by arguments
struct ProgramArgConfig
{
    bool ShowGui = false;
    std::vector<std::string> FileNames;
    std::string HepMcInFile;
};


int main(int argc, char **argv)
{

    using namespace clara;

    spdlog::info("Initializing g4e, parsing arguments...");
    //auto stdout_sink = spdlog::sinks::stdout_sink_mt::instance();
    //stdout_sink->set_level(spdlog::level::info);

    auto osink = std::make_shared<spdlog::sinks::ostream_sink_mt> (G4cout);
    spdlog::default_logger()->sinks().clear();
    spdlog::default_logger()->sinks().push_back(osink);



    ProgramArgConfig config;
    bool showHelp = false;
    auto parser = Help( showHelp )
            | Opt( config.ShowGui)
                ["--gui"]["-g"]("Shows Geant4 GUI" )
            | Opt(config.HepMcInFile,"Process hepmc files") ["--hepmc-in"]
            | Arg( config.FileNames, "<your>.mac" )
                ( "Runs Geant4 with this file" );

    parser.parse(Args(argc, argv));


    //choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    //my Verbose output class
    G4VSteppingVerbose::SetInstance(new JLeicSteppingVerbose);

    // Construct the default run manager
    auto runManager = new G4RunManager;

    auto detector = new JLeicDetectorConstruction();

    // ALICEDetectorConstruction* detector;
    // detector = new ALICEDetectorConstruction;

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new JLeicPhysicsList(detector));

    // == - set user action classes - ==

    // RUN action
    auto runAction = new JLeicRunAction(detector);
    runManager->SetUserAction(runAction);

    // Primary Generator
    auto pgAction = new JLeicPrimaryGeneratorAction(detector, runAction);
    runManager->SetUserAction(pgAction);

    // Event action
    auto eventAction = new JLeicEventAction(runAction);
    runManager->SetUserAction(eventAction);

    // Stepping action
    auto steppingAction = new JLeicSteppingAction(detector, eventAction, runAction);
    runManager->SetUserAction(steppingAction);

    // Tracking action
    auto trackingAction = new JLeicTrackingAction();
    runManager->SetUserAction(trackingAction);

    G4UImanager *UI = G4UImanager::GetUIpointer();

    if (!config.FileNames.empty())      // We have some user defined file
    {
        std::string fileName(argv[1]);
        std::string command("/control/execute " + fileName);
        UI->ApplyCommand(command);
    }

    // We start visual mode if no files provided or if --gui flag is given
    if(config.FileNames.empty() || config.ShowGui)
    {
#ifdef G4VIS_USE
        auto visManager = new G4VisExecutive;
        visManager->Initialize();
#endif

#ifdef G4UI_USE
        auto *ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
            UI->ApplyCommand("/control/execute jleicvis.mac");
    #endif
        ui->SessionStart();
        delete ui;
#endif

#ifdef G4VIS_USE
        delete visManager;
#endif
    }

    // job termination
    //
    delete runManager;

    return 0;
}
