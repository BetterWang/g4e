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

#include <signal.h>

#include "StringHelpers.hh"

#include "main_detectors/jleic/JLeicDetectorConstruction.hh"
#include "ArgumentProcessor.hh"
#include "EicPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "JLeicRunAction.hh"
#include "JLeicEventAction.hh"
#include "JLeicSteppingVerbose.hh"
#include "JLeicTrackingAction.hh"
#include "InitializationContext.hh"

#include "RootOutputManager.hh"

#include "Logging.hh"
#include "MultiActionInitialization.hh"

#include <G4MTRunManager.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <Randomize.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

//-- physics processes --
#include <FTFP_BERT.hh>
#include <QGSP_BIC.hh>
#include <TFile.h>
#include <G4GeometryManager.hh>
#include "FTFP_BERT.hh"



int main(int argc, char **argv)
{
    using namespace fmt;

    Logging::InitializeSpdLog();

    // Process user args and environment variables
    auto appArgs = InputProcessor::Process(argc, argv);
    Logging::SetGlobalLevel(appArgs.LogLevel);

    //choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    CLHEP::HepRandom::showEngineStatus();
    std::string rndmFilename(appArgs.OutputBaseName + ".bgn.rndm"); // Save state in the beginning of beginning
    CLHEP::HepRandom::saveEngineStatus(rndmFilename.c_str());

    //my Verbose output class
    G4VSteppingVerbose::SetInstance(new JLeicSteppingVerbose);

    //output root file
    std::string rootFileName(appArgs.OutputBaseName + ".root");
    std::unique_ptr<TFile> rootOutputFile(new TFile(rootFileName.c_str(), "RECREATE"));
    g4e::RootOutputManager mainRootOutput(rootOutputFile.get());

    // Construct the default run manager
    G4RunManager * runManager;
    if(appArgs.ThreadsCount > 1) {
        // Multi-threaded run manager
        auto mtRunManager = new G4MTRunManager;
        mtRunManager->SetNumberOfThreads(appArgs.ThreadsCount);
        runManager = mtRunManager;
    } else {
        // Single threaded mode
        runManager = new G4RunManager;
    }

    // Action initialization
    g4e::MultiActionInitialization actionInit;

    // Event, tracking, stepping actions
    actionInit.AddUserActionGenerator([&mainRootOutput](){return new JLeicEventAction(mainRootOutput.GetJLeicRootOutput(), mainRootOutput.GetJLeicHistogramManager());});
    actionInit.AddUserActionGenerator([&mainRootOutput](){return new JLeicRunAction(mainRootOutput.GetJLeicRootOutput(), mainRootOutput.GetJLeicHistogramManager());});
    actionInit.AddUserActionGenerator([](){return new JLeicTrackingAction();});

    // After the run manager, we can combine initialization context
    g4e::InitializationContext initContext(&appArgs, &mainRootOutput, &actionInit);

    auto detector = new JLeicDetectorConstruction(&initContext);

    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new EicPhysicsList(detector));
    runManager->SetUserInitialization(new FTFP_BERT());

    // only after we added physics lists one can add generator action
    // According to geant, 4VUserPrimaryGeneratorAction must be constructed AFTER
    // G4VUserPhysicsList is instantiated and assigned to G4RunManager.
    // At the same time we initialize it ASAP as it has an important messenger
    PrimaryGeneratorAction generatorAction;
    actionInit.SetGeneratorAction(&generatorAction);

    // Add action initialization to a run manager
    runManager->SetUserInitialization(initContext.ActionInitialization);

    // Vis manager?
    G4VisExecutive* visManager = nullptr;
    G4UIExecutive* uiExec = nullptr;
    std::string defaultMacro = "jleic.mac";     // No GUI default macro. Default macro is used if no other macros given

    // We show GUI if user didn't provided any macros of if he has --gui/-g flag
    if(appArgs.ShowGui) {
        appArgs.ShowGui = true;
        defaultMacro = "jleicvis.mac";          // Default macro for GUI
        visManager = new G4VisExecutive;
        visManager->Initialize();
        uiExec = new G4UIExecutive(argc, argv);
    }

    // auto *ui = new G4UIExecutive(argc, argv);
    G4UImanager *ui = G4UImanager::GetUIpointer();

    // set macro path from environment if it is set
    ui->ApplyCommand(format("/control/macroPath {}", appArgs.MacroPath));

    // No Macros provided by user? Use default macro
    if (appArgs.MacroFileNames.empty()) {
        appArgs.MacroFileNames.push_back(defaultMacro);
    }

    // Execute all macros
    fmt::print("Executing macro files:");
    for(const auto& fileName: appArgs.MacroFileNames) {
        std::string command = "/control/execute " + fileName;
        fmt::print("   {}\n", command);
	    std::cout << " exec command : " << command << std::endl;
        ui->ApplyCommand(command);
    }

    // We start visual mode if no files provided or if --gui flag is given
    if(appArgs.ShowGui && uiExec) {
        uiExec->SessionStart();
    }

    rndmFilename = appArgs.OutputBaseName + ".end.rndm";
    CLHEP::HepRandom::saveEngineStatus(rndmFilename.c_str());
    mainRootOutput.Write();

    G4GeometryManager::GetInstance()->OpenGeometry();
    return 0;
}
