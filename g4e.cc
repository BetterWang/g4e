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


#include <G4MTRunManager.hh>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"

#include "JLeicDetectorConstruction.hh"
#include "JLeicPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
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
#include "QGSP_BIC.hh"

#include "clara.hh"

static std::string GetResourceDir();


/// Program Configuration provided by arguments
struct ProgramArguments
{
    bool ShowGui = false;
    int ThreadsCount = 1;
    std::vector<std::string> FileNames;
};


int main(int argc, char **argv)
{
    using namespace clara;
    using namespace fmt;

    spdlog::info("Initializing g4e, parsing arguments...");
    auto osink = std::make_shared<spdlog::sinks::ostream_sink_mt> (G4cout);
    spdlog::default_logger()->sinks().clear();
    spdlog::default_logger()->sinks().push_back(osink);
    spdlog::info("Initialized G4E sink");
    spdlog::set_level(spdlog::level::debug);


    ProgramArguments args;
    bool showHelp = false;
    auto parser = Help( showHelp )
            | Opt(args.ShowGui)["--gui"]["-g"]("Shows Geant4 GUI" )
            | Opt(args.ThreadsCount)["--threads"]["-t"]("Number of threads. Single threaded mode if 0 or 1" )
            | Arg(args.FileNames, "<your>.mac" )("Runs Geant4 with this file" );
            //| Opt(args.HepMcInFile,"Process hepmc files") ["--hepmc-in"]

    parser.parse(Args(argc, argv));

    // Do we have a home environment variable?
    const char* home_cstr = std::getenv("G4E_HOME");
    std::string homeDir(home_cstr ? home_cstr : "");
    spdlog::info("ENV:G4E_HOME: '{}'", home_cstr);

    //choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    //my Verbose output class
    G4VSteppingVerbose::SetInstance(new JLeicSteppingVerbose);

    // Construct the default run manager
    G4RunManager * runManager;
    if(args.ThreadsCount > 1) {
        // Multi-threaded run manager
        auto mtRunManager = new G4MTRunManager;
        mtRunManager->SetNumberOfThreads(args.ThreadsCount);
        runManager = mtRunManager;
    } else {
        // Single threaded mode
        runManager = new G4RunManager;
    }

    auto detector = new JLeicDetectorConstruction();

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new JLeicPhysicsList(detector));

    // RUN action
    auto runAction = new JLeicRunAction(detector);
    runManager->SetUserAction(runAction);

    // Primary Generator
    auto pgAction = new PrimaryGeneratorAction();
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

    // set macro path from environment if it is set
    const char* macroPathCstr = std::getenv("G4E_MACRO_PATH");
    spdlog::info("ENV:G4E_MACRO_PATH: '{}'", macroPathCstr? macroPathCstr: "");
    if(macroPathCstr) {
        UI->ApplyCommand(format("/control/macroPath {}", macroPathCstr));
    }

    // We have some user defined file
    if (!args.FileNames.empty()) {
        spdlog::debug("Executing files provided as args:");
        for(const auto& fileName: args.FileNames) {
            std::string command = "/control/execute " + fileName;
            spdlog::debug("   {}", command);
            UI->ApplyCommand(command);
        }
    }

    // We start visual mode if no files provided or if --gui flag is given
    if(args.FileNames.empty() || args.ShowGui)
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
