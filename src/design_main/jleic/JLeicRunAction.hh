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
// $Id: JLeicRunAction.hh,v 1.5 2006-06-29 16:38:06 gunter Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef JLeicRunAction_h
#define JLeicRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <iostream>
#include "JLeicRootOutput.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class JLeicDetectorConstruction;
class JLeicHistoMConfig;
class JLeicHistogramming;

class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class JLeicRunAction : public G4UserRunAction
{
public:
    explicit JLeicRunAction(JLeicDetectorConstruction *, JLeicHistogramming *histos);

    ~JLeicRunAction() override;

public:
    void BeginOfRunAction(const G4Run *) override;

    void EndOfRunAction(const G4Run *) override;

    void CountEvent();

    void CountParticles(G4double, G4double);

    void AddEP(G4double, G4double);

    void AddEdeps(G4double Eabs);

    void AddTrackLength(G4double tlabs);

    void AddnStepsCharged(G4double ns);

    void AddnStepsNeutral(G4double ns);
    void AddTrRef(G4double tr, G4double ref);

    g4e::RootOutput mRootEventsOut;
    TFile *mHitsFile = nullptr;


private:

    G4String AnnFileName;

    G4String histName;
    JLeicDetectorConstruction *detector;

    G4double EnergySumAbs, EnergySquareSumAbs;
    G4double tlSumAbs, tlsquareSumAbs;
    G4double nStepSumCharged, nStepSum2Charged;
    G4double nStepSumNeutral, nStepSum2Neutral;
    G4double TotNbofEvents;
    G4double SumCharged, Sum2Charged, SumNeutral, Sum2Neutral;
    G4double Selectron, Spositron;

    G4double Transmitted, Reflected;

    G4double entryStep, underStep, overStep, distStep[200];
    G4double Steplow, Stephigh, dStep;
    G4int nbinStep;
    G4double entryEn, underEn, overEn, distEn[200];
    G4double Enlow, Enhigh, dEn;
    G4int nbinEn;
    G4double entryTt, underTt, overTt, distTt[200];
    G4double Ttlow, Tthigh, dTt;
    G4int nbinTt;
    G4double Ttmean, Tt2mean;
    G4double entryTb, underTb, overTb, distTb[200];
    G4double Tblow, Tbhigh, dTb;
    G4int nbinTb;
    G4double Tbmean, Tb2mean;
    G4double entryTsec, underTsec, overTsec, distTsec[200];
    G4double Tseclow, Tsechigh, dTsec;
    G4int nbinTsec;
    G4double entryTh, underTh, overTh, distTh[200];
    G4double Thlow, Thhigh, dTh;
    G4int nbinTh;
    G4double entryThback, underThback, overThback, distThback[200];
    G4double Thlowback, Thhighback, dThback;
    G4int nbinThback;
    G4double entryR, underR, overR, distR[200];
    G4double Rlow, Rhigh, dR;
    G4int nbinR;
    G4double Rmean, R2mean;
    G4double entryGamma, underGamma, overGamma, distGamma[200];
    G4double ElowGamma, EhighGamma, dEGamma;
    G4int nbinGamma;
    G4double entryvertexz, undervertexz, oververtexz, distvertexz[200];
    G4double zlow, zhigh, dz;
    G4int nbinvertexz;

    JLeicHistoMConfig *runMessenger;
    JLeicHistogramming *fHistos;
    G4int saveRndm;
};

#endif

