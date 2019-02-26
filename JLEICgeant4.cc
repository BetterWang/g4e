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
/// \file electromagnetic/VertexEIC/VertexEIC.cc
/// \brief Main program of the electromagnetic/VertexEIC example
//
//
// $Id: VertexEIC.cc 73033 2013-08-15 09:24:45Z gcosmo $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "JLeicDetectorConstruction.hh"
// #include "ALICEDetectorConstruction.hh"
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
int main(int argc,char** argv) 
{

  //choose the Random engine

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  //my Verbose output class

  G4VSteppingVerbose::SetInstance(new JLeicSteppingVerbose);
    
  // Construct the default run manager

  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes

  JLeicDetectorConstruction* detector;
  detector = new JLeicDetectorConstruction;

  // ALICEDetectorConstruction* detector;
  // detector = new ALICEDetectorConstruction;

  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new JLeicPhysicsList(detector));
 
  // set user action classes

  JLeicRunAction* runAction = new JLeicRunAction(detector); 
  runManager->SetUserAction(runAction);

  runManager->SetUserAction(new JLeicPrimaryGeneratorAction(detector,runAction));


  JLeicEventAction* eventAction = new JLeicEventAction(runAction);

  runManager->SetUserAction(eventAction);

  JLeicSteppingAction* steppingAction = new JLeicSteppingAction(detector,
                                                            eventAction, 
                                                            runAction);
  runManager->SetUserAction(steppingAction);


  runManager->SetUserAction( new JLeicTrackingAction );


  G4UImanager* UI = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode  
    {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
    }
    
  else           //define visualization and UI terminal for interactive mode
    { 
#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif    
     
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
#ifdef G4VIS_USE
      //  UI->ApplyCommand("/control/execute vis.mac");     
        UI->ApplyCommand("/control/execute jleicvis.mac");     
      //  UI->ApplyCommand("/control/execute jleic.mac");     
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
