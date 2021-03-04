#include <stdio.h>

#include "ce_EMCAL_SD.hh"
#include "ce_EMCAL_Hit.hh"
#include "ce_EMCAL_DigiHit.hh"

#include <G4VTouchable.hh>
#include <G4TouchableHistory.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Box.hh>


ce_EMCAL_SD::ce_EMCAL_SD(G4String name, g4e::RootOutputManager *rootManager) :
        G4VSensitiveDetector(name),
        fRootManager(rootManager) {

    collectionName.insert("ce_EMCAL_HitCollection");
}


void ce_EMCAL_SD::Initialize(G4HCofThisEvent *) {
    if (fVerbose > 2) {
        G4cout<<"ce_EMCAL_SD()::Initialize\n";
    }

    fHitsCollection = new ce_EMCAL_HitsCollection(SensitiveDetectorName, collectionName[0]);
    fHitsCount = 0;
}


G4bool ce_EMCAL_SD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    if (fVerbose > 2) {
        G4cout<<"ce_EMCAL_SD()::ProcessHits\n";
    }

    G4double stepl = 0.;
    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) //-- gamma ??
        stepl = aStep->GetStepLength();

    //string nameDet = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName(),

    const G4TouchableHandle touchablepre = aStep->GetPreStepPoint()->GetTouchableHandle();
    const G4TouchableHandle touchablepost = aStep->GetPostStepPoint()->GetTouchableHandle();

    // depth 1 --> x
    // depth 0 --> y
    G4int copyIDy_pre = touchablepre->GetCopyNumber();
    G4int copyIDx_pre = touchablepre->GetCopyNumber(1);
    G4int copyIDz_pre = 0;

    //JMF got crash on "run beam on"   here. Needs to be fixed ... commenting this for a moment
    G4double xstep = (aStep->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).x();
    G4double ystep = (aStep->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).y();
    G4double zstep = (aStep->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).z();
    //--- convert to the local volume --
    G4ThreeVector worldPosition = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosition = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
    G4double xinp = localPosition.x();
    G4double yinp = localPosition.y();
    G4double zinp = localPosition.z();

    G4ThreeVector worldPosition2 = aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosition2 = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition2);
    G4double xend = localPosition2.x();
    G4double yend = localPosition2.y();
    G4double zend = localPosition2.z();

    G4double xloc = (xinp + xend) / 2;
    G4double yloc = (yinp + yend) / 2;
    G4double zloc = (zinp + zend) / 2;


    if (fVerbose > 2)
        printf("--> JLeicCe_emcalSD::ProcessHits() xloc=%f yloc=%f zloc=%f  copyIDy_pre=%d \n", xloc, yloc, zloc,
               copyIDy_pre);

    if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma") {
        xloc = xend;
        yloc = yend;
        zloc = zend;
    }

    G4TouchableHistory *theTouchable = (G4TouchableHistory *) (aStep->GetPreStepPoint()->GetTouchable());


    if (fVerbose > 3)
        printf("--> JLeicCe_emcalSD::ProcessHits() Vol: 0=%s \n", theTouchable->GetVolume()->GetName().c_str());

    // process    track
    G4Track *aTrack = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4ThreeVector momentumDir = aTrack->GetMomentumDirection();
    G4int parentId = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();
    G4int PDG = aTrack->GetDefinition()->GetPDGEncoding();


    // if ((edep == 0.) && (stepl == 0.)) return false;

    //  if(mVerbose > 3)  printf("--> JLeicCe_emcalSD::ProcessHits() de=%f len=%f  IDxy=(%d,%d) step(x,y,z)=(%f, %f %f) in=(%f, %f %f) um  out=(%f, %f %f) um \n  loc=(%f, %f %f) um part=%s\n"
    //   	 ,edep,stepl/um,copyIDx_pre,copyIDy_pre,xstep,ystep,zstep,xinp/um,yinp/um,zinp/um,xend/um,yend/um,zend/um,xloc/um,yloc/um,zloc/um, aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());


    //--- save hits ------

    //  if (mVerbose > 6) printf("New CAL Hit:: IdVect=%d XYZloc (%f,%f,%f) dEdx=%f \n", aStep->GetTrack()->GetTrackID(), xloc, yloc, zloc, edep / keV);

    int curTrackID = aStep->GetTrack()->GetTrackID();
    auto track = aStep->GetTrack();

    std::string volumeName = theTouchable->GetVolume()->GetName();

    fRootManager->SaveStep(aStep, g4e::WriteStepPointChoices::PreStepPoint, copyIDx_pre, copyIDy_pre);

    if (fVerbose > 2) printf("--> JLeicCe_emcalSD::ProcessHits() Exit\n");


    G4StepPoint *prestep = aStep->GetPreStepPoint();
    G4StepPoint *poststep = aStep->GetPostStepPoint();
    G4VTouchable *TH = (G4VTouchable *) aStep->GetPreStepPoint()->GetTouchable();
    auto copyNumber = TH->GetVolume(0)->GetCopyNo();

    string name = TH->GetVolume(0)->GetName();
    double edep = aStep->GetTotalEnergyDeposit();
    double Dx = aStep->GetStepLength();
    G4ThreeVector xyz = poststep->GetPosition();
    G4ThreeVector Lxyz = prestep->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(xyz);
    double ctime = poststep->GetGlobalTime();
    int q = aTrack->GetDefinition()->GetPDGCharge();


    G4Box *box = dynamic_cast<G4Box *>(TH->GetSolid());
    // coordinate of crystal

    double x_crs = TH->GetVolume(0)->GetTranslation().x();
    double y_crs = TH->GetVolume(0)->GetTranslation().y();
    double z_crs = TH->GetVolume(0)->GetTranslation().z();

    double x_half_det = box->GetXHalfLength();
    double y_half_det = box->GetYHalfLength();
    double z_half_det = box->GetZHalfLength();


    ce_EMCAL_Hit *thisHit2 = FindExistingHit(name);

    if (!thisHit2) {

        ce_EMCAL_Hit *thisHit = new ce_EMCAL_Hit();

        thisHit->SetPos(xyz);
        thisHit->SetLPos(Lxyz);
        thisHit->SetTime(ctime);
        thisHit->SetEdep(edep / MeV);
        thisHit->SetDx(Dx);
        thisHit->SetCharge(q);
        thisHit->SetDetName(name);
        thisHit->SetCopyNumber(copyNumber);

        thisHit->SetX_crs(x_crs);
        thisHit->SetY_crs(y_crs);
        thisHit->SetZ_crs(z_crs);

        thisHit->SetXHalfLength(x_half_det);
        thisHit->SetYHalfLength(y_half_det);
        thisHit->SetZHalfLength(z_half_det);

        thisHit->SetPid(PDG);
        fHitsCollection->insert(thisHit);

    } else {

        thisHit2->SetPos(xyz);
        thisHit2->SetLPos(Lxyz);
        thisHit2->SetTime(ctime);
        thisHit2->SetEdep(edep / MeV);
        thisHit2->SetCharge(q);
        thisHit2->SetDx(Dx);
        thisHit2->SetPid(PDG);
    }


    return true;
}


void ce_EMCAL_SD::EndOfEvent(G4HCofThisEvent *HCE) {

    static G4int HCID = -1;
    if (HCID < 0) { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
    HCE->AddHitsCollection(HCID, fHitsCollection);
    //printf("--> JLeicCe_emcalSD::EndOfEvent() \n");
    // Total hits/steps per event. Set it back to 0
    /*
      mHitsCount = 0;

          JLeicCe_emcalHit *aHit ;
      JLeicCe_emcalDigiHit *dHit ;
      int nhitC = Ce_emcalCollection->GetSize();
      cout <<"Number of collections" <<nhitC<<endl;
      if(!nhitC) return;


      // Parameters Crystal

      double integration_frac = 1.;
      double optical_coupling = 0.9;
      double att_length_crs = 60000 * cm;
      double light_yield_crs = 310 * integration_frac * (1. / MeV); //Panda Crystals LY

      for (int i=0; i<nhitC; i++){

          aHit = (*Ce_emcalCollection)[i];

                  cout<<"sono prima"<<endl;
           JLeicCe_emcalDigiHit();
         JLeicCe_emcalDigiHit *dHit2=dHit->JLeicCe_emcalDigi(aHit);
          JLeicCe_emcalDigiHit(1);

      cout<<"sono dopo"<<endl;

          string name_det = dHit2->GetDetName();
          double Etot_crs = dHit2->GetEdep() ;
          double ADC_crs = dHit2->GetADC() ;
          double TDCL_crs = dHit2->GetTDC();
          double* test_pbwo=  dHit2->GetWaveform();

          cout << name_det<< " "<<Etot_crs<<" "<<ADC_crs<<endl;

          //  mRootEventsOut2->AddCe_EMCAL(name_det,Etot_crs,ADC_crs,TDCL_crs, test_pbwo);
       //   mRootEventsOut2->AddCe_EMCAL(Etot_crs,ADC_crs,TDCL_crs);
          // JLeicCe_emcalDigiHit *dHit ;

          double Etot_edep=0;
          vector<double> Edep_col = aHit->GetEdep() ;

         for(int j=0; j<aHit->GetEdep().size(); j++){
             cout<<Edep_col[j]<<endl;

             Etot_edep += Edep_col[j];
         }

     // cout << i << " "<<aHit->GetDetName()<<" "<<aHit->GetEdep().size()<<" " <<Etot_edep<< endl;

  }
      */



}


void ce_EMCAL_SD::clear() {
}


void ce_EMCAL_SD::PrintAll() {


}


ce_EMCAL_Hit *ce_EMCAL_SD::FindExistingHit(string PID)  ///< returns hit collection hit inside identifier
{
    for (size_t i = 0; i < fHitsCollection->GetSize(); i++)
        if ((*fHitsCollection)[i]->GetDetName() == PID) return (*fHitsCollection)[i];
    return nullptr;
}
