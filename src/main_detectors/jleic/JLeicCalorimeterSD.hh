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
//
// $Id: JLeicCalorimeterSD.hh,v 1.3 2006-06-29 16:37:43 gunter Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef JLeicCalorimeterSD_h
#define JLeicCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"

#include "JLeicDetectorConstruction.hh"
#include "JLeicRootOutput.hh"

//class JLeicDetectorConstruction;
//class G4HCofThisEvent;
//class G4Step;

#include "JLeicCalorHit.hh"
#include "JLeicRunAction.hh"
#include "G4RunManager.hh"

#include "rootlib.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class JLeicCalorimeterSD : public G4VSensitiveDetector
{
public:

    JLeicCalorimeterSD(G4String, JLeicDetectorConstruction *);

    ~JLeicCalorimeterSD() override;

    void Initialize(G4HCofThisEvent *) override;

    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

    void EndOfEvent(G4HCofThisEvent *) override;

    void clear() override;

    void PrintAll() override;

    void hits_reset();

private:

    //G4RunManager* fRM; //=G4RunManager::GetRunManager();
    JLeicRunAction *runaction; // = fRM->runAction

    JLeicCalorHitsCollection *CalCollection;
    JLeicDetectorConstruction *Detector;
    G4int *HitID;
    G4double dEslice[100];
    TH2F *hmatrix1[120], *hmatrix2;
    TH1F *dedx_fadc;
    int nevent;
    G4int ntr, ntr1, ntr2, ntr3;
    int N_bin, char_sh;
    TH2F *hist_charge[45 + 1];
    TFile *fm, *fm1;

    uint_fast64_t mHitsCount = 0;


    //----- EVENT STRUCTURE -----
    g4e::RootOutput* mRootEventsOut = nullptr;
    TFile * mHitsFile = nullptr;


    static const uint_fast32_t mVerbose = 0;    // verbosity. 0=none, 1=some, 2=many, 3=all

    //---------------------------
};

#endif

