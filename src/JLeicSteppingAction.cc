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
// $Id: JLeicSteppingAction.cc,v 1.6 2006-06-29 16:39:06 gunter Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "JLeicDetectorConstruction.hh"
#include "G4EnergyLossTables.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"
#include "JLeicSteppingAction.hh"
#include "JLeicPrimaryGeneratorAction.hh"
#include "JLeicEventAction.hh"
#include "JLeicRunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "JLeicSteppingMessenger.hh"
#include "G4ios.hh"
#include <iomanip>
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4INCLGlobals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JLeicSteppingAction::JLeicSteppingAction(JLeicDetectorConstruction* DET,
                                     JLeicEventAction* EA,
                                     JLeicRunAction* RA)
:detector (DET),eventaction (EA),runaction (RA),
 IDold(-1) ,evnoold(-1)
{
  //steppingMessenger = new JLeicSteppingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JLeicSteppingAction::~JLeicSteppingAction()
{
  delete steppingMessenger ;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JLeicSteppingAction::UserSteppingAction(const G4Step* aStep)
{ 

  G4double Theta,Thetaback,Ttrans,Tback,Tsec,Egamma,DEgamma,xend,yend,zend,rend,xp,yp,zp,rp ;
  G4int evno = eventaction->GetEventno() ; 

  IDnow = evno+10000*(aStep->GetTrack()->GetTrackID())+
    100000000*(aStep->GetTrack()->GetParentID());                //-- 100 k events only ???

  /* printf("SteppingAction:: Volume=%s  x=%f y=%f z=%f   mom (%f,%f, %f ) \n",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(),
  	 aStep->GetTrack()->GetPosition().x(), aStep->GetTrack()->GetPosition().y(), aStep->GetTrack()->GetPosition().z(),
         aStep->GetTrack()->GetMomentumDirection().x(),aStep->GetTrack()->GetMomentumDirection().y(),aStep->GetTrack()->GetMomentumDirection().z());
  */

  //--------------------------for TUNE-------------------------------------------------------------------------
  if( strcmp(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(),"Physics_QUADS_hd_m_iQDS1a")==0){


 char myftune[256];
 float my_or;
 // printf("read Di file\n");
 sprintf(myftune,"tune.txt");
 rc=fopen(myftune,"w");
 if (rc == NULL) return;
 //
 // (x+0.3529979921)^2+(px+200*tan(0.05299800449))^2+y^2+py^2 where x, px, y, py are at the entrance into iQDS1a
 //
//   my_or= (aStep->GetTrack()->GetPosition().x()+0.3529979921)*(aStep->GetTrack()->GetPosition().x()+0.3529979921) 
 //       +( aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))*( aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))
 //       +aStep->GetTrack()->GetPosition().y()*aStep->GetTrack()->GetPosition().y()
 //       +aStep->GetTrack()->GetMomentumDirection().y()*aStep->GetTrack()->GetMomentumDirection().y(); 
   my_or= (aStep->GetTrack()->GetPosition().x()/100.+0.3529979921)*(aStep->GetTrack()->GetPosition().x()/100.+0.3529979921) 
        +(200*aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))*(200*aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))
     +(aStep->GetTrack()->GetPosition().y()/100.)*(aStep->GetTrack()->GetPosition().y()/100.)
     +(200*aStep->GetTrack()->GetMomentumDirection().y())*(200*aStep->GetTrack()->GetMomentumDirection().y()); 

   //   fprintf(rc," %f %f %f %f %f %f \n", aStep->GetTrack()->GetPosition().x(), aStep->GetTrack()->GetPosition().y(), aStep->GetTrack()->GetPosition().z(),  aStep->GetTrack()->GetMomentumDirection().x(),aStep->GetTrack()->GetMomentumDirection().y(),aStep->GetTrack()->GetMomentumDirection().z());
      printf("SteppingAction:: %f \n", my_or );
      fprintf(rc,"%f", my_or );
      fclose(rc);
     
    }
 //--------------------------end for TUNE-------------------------------------------------------------------------

  if(IDnow != IDold)
  {
   IDold=IDnow ;
   //printf(" primary=%s\n",JLeicPrimaryGeneratorAction::GetPrimaryName().c_str());
   //-----------------
   //-- e+ e- vertex 
   if(
      (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e-") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) )
       ||
      (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e+") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) )
     )
     runaction->Fillvertexz(aStep->GetTrack()->GetVertexPosition().z()); //-- Z ?
   //-----------


   if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber")
   {
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e-") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) ) 
    {
        eventaction->AddCharged() ;
        eventaction->AddE() ;
        Tsec = aStep->GetTrack()->GetKineticEnergy() ;  // !!!!!!!!!!!!
        //Tsec += aStep->GetTotalEnergyDeposit() ;        // !!!!!!!!!!!!
        runaction->FillTsec(Tsec/keV) ;
    }
    else
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e+") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) ) 
    {
        eventaction->AddCharged() ;
        eventaction->AddP() ;
        Tsec = aStep->GetTrack()->GetKineticEnergy() ;  // !!!!!!!!!!!!
        //Tsec += aStep->GetTotalEnergyDeposit() ;        // !!!!!!!!!!!!
        runaction->FillTsec(Tsec/keV) ;
    }
    else  //-- gamma START in absorber 
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "gamma") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) ) 
    {
        eventaction->AddNeutral() ;
        //eventaction->AddGammaDE(aStep->GetTotalEnergyDeposit()) ;
	Egamma = aStep->GetTrack()->GetKineticEnergy() ;
	//printf("SEC gammas FROM absorber E=%f dE=%f ID=%d/%d ",Egamma,aStep->GetTotalEnergyDeposit(),IDnow,IDold);
	//printf("  >  created by %s\n",aStep->GetTrack()->GetCreatorProcess()->GetProcessName().c_str());
    }

   } //-- end pxdPixel
  }  //-- (IDnow != IDold)

  //printf(" Volume=%s  \n",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str());

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //    material budget calculation
  // In examples/advanced/amsEcal, among other things, there is a geantino scan.
  // The result is plotted in term of nb of radiation length crossed.
  // Geant4 has "/control/matScan" commands which measure thickness in
  // terms of length, radiation length and interaction length. These
  // commands do not sort lengths out for each material, but they do
  // for each region. Thus, by defining each logical volume as unique
  // region, you should get all you need.
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //if world, return
  //
  /*
  if (lvol == lvol_world) return;
  
  //sum nb of radiation length of calorimeter with geantino
  //
  G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();
  if (particle == G4Geantino::Geantino()) {
    G4double radl  = lvol->GetMaterial()->GetRadlen();
    G4double stepl = step->GetStepLength();
    eventAct->SumNbRadLength(stepl/radl);
  }
  */
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    

  //----------------------------------------------------------------------------
  //-- STEP in pxdPixel 
  if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="svdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber")
  {

    printf("step in absorber de=%f  step=%f Particle=%s energy=%f [mev]\n",aStep->GetTotalEnergyDeposit()/keV,aStep->GetStepLength()/um,
   	   aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str(), aStep->GetTrack()->GetKineticEnergy()/MeV);

    zend= aStep->GetTrack()->GetPosition().z() ;
    runaction->FillHist(7,zend-detector->fAbsorberZ+detector->fAbsorberThickness/2.,aStep->GetTotalEnergyDeposit()/keV);
    if(  ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
	   GetParticleName()) == "gamma")) {
         runaction->FillHist(27,aStep->GetTotalEnergyDeposit()/keV,1.);
    } else {
     if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel"){
       runaction->FillHist(25,aStep->GetTotalEnergyDeposit()/keV,1.);
       runaction->FillHist2d(2, aStep->GetTrack()->GetKineticEnergy()/MeV,aStep->GetTotalEnergyDeposit()/keV,1.);}
     else if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="svdPixel"){
      runaction->FillHist(26,aStep->GetTotalEnergyDeposit()/keV,1.);
      runaction->FillHist2d(3, aStep->GetTrack()->GetKineticEnergy()/MeV,aStep->GetTotalEnergyDeposit()/keV,1.);
     }
    }
   //if ( 201 < zend && zend < 203 )  runaction->FillHist(11,aStep->GetTotalEnergyDeposit()/keV);
    //if ( 225 < zend && zend < 227 )  runaction->FillHist(12,aStep->GetTotalEnergyDeposit()/keV);
    //if ( 235 < zend && zend < 237 )  runaction->FillHist(13,aStep->GetTotalEnergyDeposit()/keV);

    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e-") 
              ||
       ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
	 GetParticleName()) == "e+"))  {

          eventaction->CountStepsCharged() ;

	  runaction->FillHist(3,aStep->GetStepLength()/um  );
	  if (  (aStep->GetTrack()->GetTrackID() == 1) && (aStep->GetTrack()->GetParentID()== 0)  ) {	    
	    runaction->FillHist(5,zend-detector->fAbsorberZ+detector->fAbsorberThickness/2.,aStep->GetTotalEnergyDeposit()/keV);
	  } else {
	    runaction->FillHist(4,zend-detector->fAbsorberZ+detector->fAbsorberThickness/2.,aStep->GetTotalEnergyDeposit()/keV);
	  }
	  runaction->FillHist(6,zend-detector->fAbsorberZ+detector->fAbsorberThickness/2.,aStep->GetTotalEnergyDeposit()/keV);
    }
    if ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
	 GetParticleName()) == "gamma") 
    {
      eventaction->CountStepsNeutral() ;
	DEgamma=aStep->GetTotalEnergyDeposit();
      //eventaction->AddGammaDE(aStep->GetTotalEnergyDeposit()) ;
      if (DEgamma>0.00000001)
      runaction->FillGamDE(DEgamma) ;


      G4TouchableHandle theTouchable = aStep->GetPreStepPoint()->GetTouchableHandle();
      G4int copyNo = theTouchable->GetCopyNumber();

      G4int histDepth = theTouchable->GetHistoryDepth();
      G4int motherCopyNo = theTouchable->GetCopyNumber(1);
      G4int motherRepNo = theTouchable->GetReplicaNumber(1);

  // depth 1 --> x
  // depth 0 --> y
  G4int copyIDy_pre  = theTouchable->GetCopyNumber();
  G4int copyIDx_pre  = theTouchable->GetCopyNumber(1);
  //if (DEgamma>0.00000001)
  printf("UserSteppingAction():: gamma: E=%f dep=%f histDepth=%d moCopy=%d, moRep=%d IDxy=(%d,%d)\n",aStep->GetTrack()->GetKineticEnergy()/keV,aStep->GetTotalEnergyDeposit()/keV,histDepth,motherCopyNo,motherRepNo,copyIDx_pre,copyIDy_pre);

     if (DEgamma>0.00000001) {
       runaction->FillGammaEStep(DEgamma) ;
       xend= aStep->GetTrack()->GetPosition().x() ;
       yend= aStep->GetTrack()->GetPosition().y() ;
       zend= aStep->GetTrack()->GetPosition().z() ;
       runaction->FillHist(1,zend-detector->fAbsorberZ+detector->fAbsorberThickness/2. );
       runaction->FillHist(2,zend-detector->fAbsorberZ+detector->fAbsorberThickness/2., DEgamma/keV );
     }
      //G4VProcess* creatorProcess = aStep->GetTrack()->GetCreatorProcess();      
      //if(creatorProcess->GetProcessName() == “GammaXTRadiator”) G4cout << “Particle was created by the LowEnergy” << “Ionization process“ << G4endl;
      //printf(" Gamma greated by %s\n",aStep->GetTrack()->GetCreatorProcess()->GetProcessName().c_str());
      //printf(" ALL gammas STEP in absorber de=%f ID=%d/%d ParID=%d TrkID=%d\n",aStep->GetTotalEnergyDeposit(),IDnow,IDold
      //     ,aStep->GetTrack()->GetParentID(),aStep->GetTrack()->GetTrackID());
    }
  } //-- if absorber 
  else {

    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() !=0 )
	  runaction->FillHist(3,aStep->GetStepLength()/um  );
  }


  //----------------------------------------------------------------------------
  //-- Primary EXIT absorber forward ? GetMomentumDirection().x()>0. !!! x !!!  ERROR !! .z !!!
  if (
      (((aStep->GetTrack()->GetTrackID() == 1) &&
        (aStep->GetTrack()->GetParentID()== 0)) ||
        (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName() ==
        JLeicPrimaryGeneratorAction::GetPrimaryName())) 
        &&
        (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber")
        &&
        (aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
        (aStep->GetPostStepPoint()->GetProcessDefinedStep()
              ->GetProcessName() == "Transportation") &&
        (aStep->GetTrack()->GetMomentumDirection().z()>0.)
                                                        )
     {
       eventaction->SetTr();
       Theta = std::acos(aStep->GetTrack()->GetMomentumDirection().x()) ;
       runaction->FillTh(Theta) ;
       Ttrans = aStep->GetTrack()->GetKineticEnergy() ;
       runaction->FillTt(Ttrans) ;
       yend= aStep->GetTrack()->GetPosition().y() ;
       zend= aStep->GetTrack()->GetPosition().z() ;
       rend = std::sqrt(yend*yend+zend*zend) ;
       runaction->FillR(rend);
     }
  //----------------------------------------------------------------------------
  //--  Primary EXIT absorber backward ? GetMomentumDirection().z()<0. ---
  if (
      (((aStep->GetTrack()->GetTrackID() == 1) &&
        (aStep->GetTrack()->GetParentID()== 0)) ||
        (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == JLeicPrimaryGeneratorAction::GetPrimaryName())) 
       &&
      (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber") &&
      (aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
      (aStep->GetPostStepPoint()->GetProcessDefinedStep()
               ->GetProcessName() == "Transportation") &&
      (aStep->GetTrack()->GetMomentumDirection().z()<0.)
                                                        )
     {
       eventaction->SetRef();
       Thetaback = std::acos(aStep->GetTrack()->GetMomentumDirection().x()) ;
       Thetaback -= 0.5*CLHEP::pi ;
       runaction->FillThBack(Thetaback) ;
       Tback  = aStep->GetTrack()->GetKineticEnergy() ;
       runaction->FillTb(Tback) ;
     }
 
  //----------------------------------------------------------------------------
  //--  gamma EXIT absorber forward
  if (aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary && aStep->GetPostStepPoint()->GetPhysicalVolume() )    {
  if (
      ((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber") &&
       (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="World") &&
       //(aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
      (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation") &&
      (aStep->GetTrack()->GetMomentumDirection().z()>0.) &&
      (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma"))
     )
     {
       Egamma = aStep->GetTrack()->GetKineticEnergy() ;
       runaction->FillGammaOutSpectrum(Egamma) ;
       //printf("O::test PreStep = %s ",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str());
       //printf("O::test PostStep = %s \n",aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().c_str());
       //printf("E::gammas EXIT absorber de=%f E=%f step=%f\n",aStep->GetTotalEnergyDeposit(),Egamma/keV,aStep->GetStepLength());
     }
  }

  //--  gamma ENTER absorber ???

  //printf("----------- test ----------------- fGeomBoundary-%d(%d) \n",aStep->GetPostStepPoint()->GetStepStatus(),fGeomBoundary);

  if (aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary && aStep->GetPostStepPoint()->GetPhysicalVolume() )    {
    if (
	(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="World") &&
	( (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="Absorber") ||
	  (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="pxdPixel") ) &&
	(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation") &&
	(aStep->GetTrack()->GetMomentumDirection().z()>0.)                       &&
	(aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma")
	)
      {
	Egamma = aStep->GetTrack()->GetKineticEnergy() ;
	runaction->FillGammaInSpectrum(Egamma) ;
	//printf("I::test PreStep = %s ",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str());
	//printf("I::test PostStep = %s \n",aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().c_str());
	//printf("I::gammas ENTER absorber de=%f E=%f ID=%d\n",aStep->GetTotalEnergyDeposit()/keV,Egamma/keV,IDnow);

	xp=aStep->GetTrack()->GetMomentumDirection().x();
	yp=aStep->GetTrack()->GetMomentumDirection().y();
	zp=aStep->GetTrack()->GetMomentumDirection().z();
	rp=std::sqrt(yp*yp+xp*xp);
	Theta = std::acos(zp) ;
	runaction->FillGamAngle(Theta) ;
	Ttrans = aStep->GetTrack()->GetKineticEnergy() ;
	runaction->FillTt(Ttrans) ;
	xend= aStep->GetTrack()->GetPosition().x() ;
	yend= aStep->GetTrack()->GetPosition().y() ;
	zend= aStep->GetTrack()->GetPosition().z() ;
	rend = std::sqrt(yend*yend+zend*zend) ;
      }
  }
  //------------------  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

