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

#include "StringHelpers.hh"

#include "MulticastEventAction.hh"

#include "main_detectors/jleic/JLeicDetectorConstruction.hh"
#include "InputProcessor.hh"
#include "JLeicPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "JleicHistogramming.hh"
#include "JLeicRunAction.hh"
#include "JLeicEventAction.hh"
#include "JLeicSteppingAction.hh"
#include "JLeicSteppingVerbose.hh"
#include "JLeicTrackingAction.hh"

#include "MulticastEventAction.hh"
#include "MulticastSteppingAction.hh"
#include "Logging.hh"

#include <G4MTRunManager.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <Randomize.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

//-- physics processes --
#include <FTFP_BERT.hh>
#include <QGSP_BIC.hh>


int main(int argc, char **argv)
{
    using namespace fmt;

    if( argc > 1 ) {
        std::cout << "there are " << argc-1 << " (more) arguments, they are:\n" ;
        std::copy( argv+1, argv+argc, std::ostream_iterator<const char*>( std::cout, "\n" ) ) ;
    }

    Logging::InitializeSpdLog();

    // Process user args and environment variables
    auto args = InputProcessor::Process(argc, argv);
    Logging::SetGlobalLevel(args.LogLevel);


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
    auto jleicHistos = new JLeicHistogramming();

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new JLeicPhysicsList(detector));

    // RUN action
    auto runAction = new JLeicRunAction(detector, jleicHistos);
    runManager->SetUserAction(runAction);

    // Primary Generator
    auto pgAction = new PrimaryGeneratorAction();
    runManager->SetUserAction(pgAction);

    // Event action
    auto eventAction = new MulticastEventAction();
    auto jleicEventAction = new JLeicEventAction(runAction, jleicHistos);
    eventAction->AddUserAction(jleicEventAction);

    runManager->SetUserAction(eventAction);

    // Stepping action

    auto steppingAction = new JLeicSteppingAction(detector, jleicEventAction, runAction, jleicHistos);
    runManager->SetUserAction(steppingAction);

    // Tracking action
    auto trackingAction = new JLeicTrackingAction();
    runManager->SetUserAction(trackingAction);

    // Vis manager?
    G4VisExecutive* visManager = nullptr;
    G4UIExecutive* uiExec = nullptr;
    std::string defaultMacro = "jleic.mac";     // No GUI default macro. Default macro is used if no other macros given

    // We show GUI if user didn't provided any macros of if he has --gui/-g flag
    if(args.MacroFileNames.empty() || args.ShowGui) {
        args.ShowGui = true;
        defaultMacro = "jleicvis.mac";          // Default macro for GUI
        visManager = new G4VisExecutive;
        visManager->Initialize();
        uiExec = new G4UIExecutive(argc, argv);
    }

    // auto *ui = new G4UIExecutive(argc, argv);
    G4UImanager *ui = G4UImanager::GetUIpointer();

    // set macro path from environment if it is set
    ui->ApplyCommand(format("/control/macroPath {}", args.MacroPath));

    // No Macros provided by user? Use default macro
    if (args.MacroFileNames.empty()) {
        args.MacroFileNames.push_back(defaultMacro);
    }

    // Execute all macros
    fmt::print("Executing macro files:");
    for(const auto& fileName: args.MacroFileNames) {
        std::string command = "/control/execute " + fileName;
        fmt::print("   {}\n", command);
        ui->ApplyCommand(command);
    }

    // We start visual mode if no files provided or if --gui flag is given
    if(args.ShowGui && uiExec) {
        uiExec->SessionStart();
    }

    return 0;
}
