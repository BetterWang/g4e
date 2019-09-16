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
//
#include <stdio.h>
#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


//=====================================================================================================
static int save_hits_root = 1;
static int use_depfet = 1;
static int use_fdc = 0;

static FILE *fann; //*fopen(const char *path, const char *mode);
static char AnnFileName[128];
static int NVAR;
static int jDebug = 3;

//=====================================================================================================

#include "JLeicVertexSD.hh"

#include "JLeicVTXHit.hh"
//#include "JLeicDetectorConstruction.hh"
//#include "G4Step.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//--- my 
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JLeicVertexSD::JLeicVertexSD(G4String name, JLeicDetectorConstruction *det)
        : G4VSensitiveDetector(name), Detector(det)
{
    printf("JLeicVertexSD()::constructor  enter\n");
    //fRM=G4RunManager::GetRunManager();

    NumRow = 10; // :64 depfet
    NumCol = 50; // :256 depfet
    FRAME.resize(NumRow*NumCol);
    nevent = 0;
    collectionName.insert("VTXCollection");
    HitID = new G4int[500];
    printf("--> JLeicVertexSD::Constructor(%s) \n", name.c_str());

    if (save_hits_root) {
        mHitsFile = new TFile("g4e_output.root", "RECREATE");
        mRootEventsOut.Initialize(mHitsFile);
    }


    if (use_depfet) for (int ii = 0; ii < (NumRow * NumCol); ii++) FRAME[ii] = 0; //-- 8000;  //-- set pedestals
    if (use_fdc) {
        dedx_fadc = new TH1F("dedx_fadc", " FDC dE/dX", 50, -0.5, 49.5);
        printf(" SAVE Matrix file trd_frames.root \n");
        fm1 = new TFile("fdc_frames.root", "RECREATE");
        for (int ii = 0; ii < 100; ii++) dEslice[ii] = 0; // reset
    }
    //-----------------  charge sharing hist ------
    char_sh = 0;
    N_bin = 45;
    char hname1[80], hname2[80];
    G4double Max_Sigm = 0.55;  //-- at 450 um
    if (use_depfet && char_sh) //--  diffusion
        for (int iz = 0; iz <= N_bin; iz++) {

            G4double mean1 = 0., mean2 = 0., sig1 = iz * Max_Sigm / N_bin, sig2 = iz * Max_Sigm / N_bin;

            sprintf(hname1, "hist_charge_%d", iz);
            sprintf(hname2, "charge sharting sigm=%f; Xpos ; Ypos;", sig1);
            hist_charge[iz] = new TH2F(hname1, hname2, 100, -5., +5., 100, -5., +5.); //-- 5x5 pixels

            for (int ig = 0; ig < 1000000; ig++) {
                G4double g1rnd = G4RandGauss::shoot(mean1, sig1);
                G4double g2rnd = G4RandGauss::shoot(mean2, sig2);
                hist_charge[iz]->Fill(g1rnd, g2rnd);
            }

            G4double cbin = hist_charge[iz]->GetBinContent(51, 51);
            printf("--> JLeicVertexSD::Constructor():: iz=%d, sigm=%f hist_charge(51,51)=%f \n", iz, sig1, cbin);
        }

    //----------------------------------------------------
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JLeicVertexSD::~JLeicVertexSD() {


    if(mHitsFile)
    {
        mHitsFile->cd();
        mRootEventsOut.Write();
        mHitsFile->Close();
    }


    delete[] HitID;

    printf("JLeicVertexSD():: Delete dedx_fadc ...  \n");

    if (use_fdc) delete dedx_fadc;

    printf("JLeicVertexSD():: Deleted dedx_fadc ...  \n");


    printf("JLeicVertexSD():: Done ...  \n");
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JLeicVertexSD::Initialize(G4HCofThisEvent *) {

  printf("JLeicVertexSD()::Initialize enter nevent=%d  Lays.size()=%d\n", nevent,Detector->cb_VTX.Lays.size());

    VTXCollection = new JLeicVTXHitsCollection
            (SensitiveDetectorName, collectionName[0]);
    for (G4int j = 0; j < 1; j++) { HitID[j] = -1; };

    if (nevent == 0) {
        runaction = (JLeicRunAction *) (G4RunManager::GetRunManager()->GetUserRunAction());
        //
        //  NVAR=Detector->NannVAR;
        //  sprintf(AnnFileName,"%s_nv%d.dat",runaction->GetAnnFileName().c_str(),NVAR);
        // printf("AnnFileName=%s\n",AnnFileName);
        // if((fann = fopen(AnnFileName,"w")) == NULL)  { printf("Can not open file %s \n",AnnFileName); };
        //    fprintf(fann,"    %d\n",NVAR);
        //  for (int ii=0;ii<NVAR;ii++) {
        //  fprintf(fann,"E%d\n",ii);
        // }
        //
        printf("JLeicVertexSD():: First Event ev=%d AnnFileName=%s \n", nevent, AnnFileName);
    }
    nevent++;
    if (use_fdc) {
        for (int ii = 0; ii < 100; ii++) dEslice[ii] = 0; // reset
        if (!(nevent % Detector->fModuleNumber)) { //-- number of chambers simple aproximation ----
            //      for (int ii=0; ii<100; ii++) dEslice[ii]=0; // reset
            ntr = 0;
            ntr1 = 0;
            ntr2 = 0;
            ntr3 = 0;
            //printf("reset energy \n");
        }
    }
    //printf("JLeicVertexSD()::Initialize 1\n");

    //printf("--> JLeicVertexSD::Initialize(%s) \n",collectionName[0].c_str());

    if (save_hits_root) {
        mRootEventsOut.ClearForNewEvent();
    }



    //printf("JLeicVertexSD()::Initialize 2\n");
    if (use_depfet) for (int ii = 0; ii < (NumRow * NumCol); ii++) FRAME[ii] = 0; //-- 8000;  //-- reset pedestals
    if (use_fdc) dedx_fadc->Reset(" ");

    mHitsCount = 0;
    printf("JLeicVertexSD()::Initialize exit\n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool JLeicVertexSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    if (jDebug > 2) printf("--> JLeicVertexSD::ProcessHits() Enter\n");



    //  const G4TouchableHandle touchablepre[128];
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4double stepl = 0.;
    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) //-- gamma ??
        stepl = aStep->GetStepLength();


    const G4TouchableHandle touchablepre = aStep->GetPreStepPoint()->GetTouchableHandle();
    const G4TouchableHandle touchablepost = aStep->GetPostStepPoint()->GetTouchableHandle();
    // depth 1 --> x
    // depth 0 --> y
    G4int copyIDy_pre = touchablepre->GetCopyNumber();
    G4int copyIDx_pre = touchablepre->GetCopyNumber(1);
    G4int copyIDz_pre = 0;
    if (use_depfet > 0) copyIDz_pre = touchablepre->GetCopyNumber(2);
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

    if (jDebug > 2) printf("xloc=%f yloc=%f zloc=%f  \n", xloc, yloc, zloc);
    if (use_fdc) {  //----- FDC / TRD  ---

        int zbin = (zloc / mm + Detector->GetAbsorberThickness() / 2.) /
                   Detector->fadc_slice; //-- z position , slice number
        //int zbin = (zloc/mm+Detector->GetAbsorberThickness()/2.) /  (Detector->GetAbsorberThickness()/10.)  ; //-- z position , slice 1/10
        //printf("zbin=%d zloc=%f zinp=%f  zend=%f wz=%f \n",zbin,zloc,zinp,zend,worldPosition.z());


        if (0 <= zbin && zbin < 100) dEslice[zbin] += edep / keV;
        dedx_fadc->Fill(zbin, edep / keV);
    }


    if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma") {
        xloc = xend;
        yloc = yend;
        zloc = zend;
    }

    //---------digitizing------------------------------

    G4double gain = 1400. * 1.055;
    G4double sigma_gain = 8. / 100.;
    gain = G4RandGauss::shoot(gain, sigma_gain / gain);
    short int ADC = edep / keV * gain / 59.5;

    if (char_sh == 0 && copyIDy_pre < NumRow && copyIDx_pre < NumCol) FRAME[copyIDy_pre * NumCol + copyIDx_pre] += ADC;

    G4TouchableHistory *theTouchable
            = (G4TouchableHistory *) (aStep->GetPreStepPoint()->GetTouchable());

    if (jDebug > 3)
        printf("--> JLeicVertexSD::ProcessHits() Vol: 0=%s \n", theTouchable->GetVolume()->GetName().c_str());

    if (jDebug > 3)
        printf("--> JLeicVertexSD::ProcessHits() Vol: 0=%s 1=%s  2=%s 3=%s Abs=%s\n",
               theTouchable->GetVolume()->GetName().c_str(), theTouchable->GetVolume(1)->GetName().c_str(),
               theTouchable->GetVolume(2)->GetName().c_str(), theTouchable->GetVolume(3)->GetName().c_str(),
               Detector->GetAbsorber()->GetName().c_str());

    if (use_depfet > 0) {
        G4String VTXmod = theTouchable->GetVolume()->GetName();
        if (jDebug > 2) printf("VTX_ladder=%s \n", VTXmod.c_str());
        //   int Mod=int (VTXmod<<10);
        // printf("VTX_ladder=%d \n",Mod);
        if (jDebug > 2)
            printf("--> JLeicVertexSD::ProcessHits() de=%f ADC=%d  len=%f Mod=%s  IDxy=(%d,%d,%d)\n", edep / keV,
                   ADC, stepl / um, VTXmod.c_str(), copyIDx_pre, copyIDy_pre, copyIDz_pre);
        for (int in = 0; in < 12; in++) {
            sprintf(buffer, "VTX_ladder1_%d", in);
            if (strcmp(VTXmod.c_str(), buffer) == 0) {
                runaction->FillHistmatrixOccup(in, copyIDx_pre, copyIDy_pre, edep / keV);
                runaction->FillHistmatrixOccupCM(in, xstep, ystep, edep / keV);
                if (jDebug > 2) printf("xstep=%f ystep=%f \n", xstep, ystep);
            }
        }
    }
    //-------------------------------------------------------------------------------------
    if (char_sh > 0) {
        int iz = (225 - zloc / um) / 10;
        if (iz > N_bin) iz = N_bin;
        if (iz < 0) iz = 0;
        int ix = (xloc / um) / 20. * 10;
        if (ix > 5) ix = 5;
        if (ix < -5) ix = -5;
        int iy = (yloc / um) / 20. * 10;
        if (iy > 5) iy = 5;
        if (iy < -5) iy = -5;

        G4double sum = 0, total = 0;
        int ic = 0, ky = 0, kx = 0, i0 = 51, j0 = 51;
        for (int i = (i0 - 25); i <= (i0 + 15); i += 10) {
            kx = kx + 1;
            ky = 0;
            for (int j = (j0 - 25); j <= (j0 + 15); j += 10) {
                ky = ky + 1;
                sum = 0;
                for (int k = 0; k <= 9; k++) {
                    for (int n = 0; n <= 9; n++) {
                        sum = sum + hist_charge[iz]->GetBinContent(i + k + ix, j + n + iy) * 1e-6; //HIJ(30,i+k,j+n);
                        //printf(" chan=%d %d\n",i-i0+k,j-j0+n);
                    }
                }
                int ycell = copyIDy_pre - 3 + ky;
                int xcell = copyIDx_pre - 3 + kx;
                ADC = edep * sum / keV * gain / 59.5;
                if (0 <= ycell && ycell < NumRow && 0 < xcell && xcell < NumCol) FRAME[ycell * NumCol + xcell] += ADC;
                total = total + sum;
                ic = ic + 1;
                //printf(" iz=%d (%d,%d) ic=%d x=%d y=%d sum=%f kx=%d ky=%d\n", iz,ix,iy,ic,i-i0,j-j0,sum, kx, ky);
            }
        }
        //printf("total=%f \n", total);
    }
    //-------------------------------------------------------------------------------------

    // process
    //const G4VProcess * aProcessPointer = aStep->GetPostStepPoint()->GetProcessDefinedStep();
    // track
    G4Track *aTrack = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4ThreeVector momentumDir = aTrack->GetMomentumDirection();
    G4int ParrentID = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();
    G4int PDG = aTrack->GetDefinition()->GetPDGEncoding();

    //vertex (,where this track was created) information
    /*
      const G4ThreeVector& GetVertexPosition() const;
      void  SetVertexPosition(const G4ThreeVector& aValue);
      const G4ThreeVector& GetVertexMomentumDirection() const;
      G4int GetCurrentStepNumber() const;
      void  IncrementCurrentStepNumber();
    */


    if ((edep == 0.) && (stepl == 0.)) return false;

    //  printf("--> JLeicVertexSD::ProcessHits() de=%f ADC=%d  len=%f  IDxy=(%d,%d) step(x,y,z)=(%f, %f %f) in=(%f, %f %f) um  out=(%f, %f %f) um \n  loc=(%f, %f %f) um part=%s\n"
    //	 ,edep/keV,ADC,stepl/um,copyIDx_pre,copyIDy_pre,xstep,ystep,zstep,xinp/um,yinp/um,zinp/um,xend/um,yend/um,zend/um,xloc/um,yloc/um,zloc/um, aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());


    //--- save hits ------
    if (save_hits_root) {
        if (jDebug > 6)
            printf("New Hit:: IdVect=%d XYZloc (%f,%f,%f) dEdx=%f \n", aStep->GetTrack()->GetTrackID(), xloc, yloc, zloc,
                   edep / keV);

        int curTrackID = aStep->GetTrack()->GetTrackID();
        std::string volumeName = theTouchable->GetVolume()->GetName().c_str();
        mRootEventsOut.AddHit(
                 mHitsCount,  /* aHitId */
                 curTrackID,  /* aTrackId */
                 xstep / mm,  /* aX */
                 ystep / mm,  /* aY */
                 zstep / mm,  /* aZ */
                 edep / GeV,  /* aELoss */
                copyIDx_pre,  /* aIRep */
                copyIDy_pre,  /* aJRep */
                volumeName    /* aVolNam */
               );
        mHitsCount++;

        //-- fill tracks --
        mRootEventsOut.AddTrack(
                curTrackID,                           /* int aTrackId,*/
                ParrentID,                            /* int aParentId,*/
                PDG,                                  /* int aTrackPdg,*/
                vertex.x() / mm,                      /* double aXVertex,*/
                vertex.y() / mm,                      /* double aYVertex,*/
                vertex.z() / mm,                      /* double aZVertex,*/
                vertexMom.x(),                        /* double aXMom,*/
                vertexMom.y(),                        /* double aYMom,*/
                vertexMom.z(),                        /* double aZMom,*/
                momentum.mag() / GeV                  /* double aMom*/
        );

    } //  if (save_hits_root)





    G4VPhysicalVolume *physVol = theTouchable->GetVolume();
    //theTouchable->MoveUpHistory();
    G4int JLeicNumber = 0;

    if (jDebug > 2)
        printf("--> JLeicVertexSD::ProcessHits() Vol=(%s) %p Abs=%p \n", physVol->GetName().c_str(),
               (void *) physVol, (void *) Detector->GetAbsorber());

    if (HitID[JLeicNumber] == -1) {
        JLeicVTXHit *vtxHit = new JLeicVTXHit();
        if (physVol == Detector->GetAbsorber()) vtxHit->AddAbs(edep, stepl);
        HitID[JLeicNumber] = VTXCollection->insert(vtxHit) - 1;
        if (verboseLevel > 0)
            G4cout << " New Vertex Hit on JLeic: " << JLeicNumber << G4endl;
        //printf("--> JLeicVertexSD::ProcessHits()  New Vertex Hit on JLeic: %d de=%f\n",JLeicNumber,edep/keV);
    } else {
        if (physVol == Detector->GetAbsorber())
            (*VTXCollection)[HitID[JLeicNumber]]->AddAbs(edep, stepl);
        if (verboseLevel > 0)
            G4cout << " Energy added to JLeic: " << JLeicNumber << G4endl;
        //printf("--> JLeicVertexSD::ProcessHits()  Energy added to JLeic: %d  de=%f\n",JLeicNumber,edep/keV);
    }

    if (jDebug > 2) printf("--> JLeicVertexSD::ProcessHits() Exit\n");

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JLeicVertexSD::EndOfEvent(G4HCofThisEvent *HCE) {

    static G4int HCID = -1;
    if (HCID < 0) { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
    HCE->AddHitsCollection(HCID, VTXCollection);
    //printf("--> JLeicVertexSD::EndOfEvent() \n");

    if (use_fdc) {

        if (NVAR > Detector->GetAbsorberThickness() / Detector->fadc_slice) {
            printf("Error FADC slices:: NVAR=%d Slices=%f \n", NVAR,
                   Detector->GetAbsorberThickness() / Detector->fadc_slice);
            exit(1);
        }

        fprintf(fann, "    %d\n", NVAR);
        for (int ii = 0; ii < NVAR; ii++) {
            fprintf(fann, "%.2f ", dEslice[ii]);
        }
        fprintf(fann, "\n");

        G4double etot = 0;
        for (int ii = 0; ii < 100; ii++) {
            etot += dEslice[ii];
            runaction->FillHist2d(1, dEslice[ii], (double) ii, 1.);
            runaction->FillHLikelihood(ii, dEslice[ii]);
        }
        runaction->FillHist(10, etot);
        for (int ii = 0; ii < 3; ii++) runaction->FillHist(11, dEslice[ii]);
        for (int ii = 20; ii < 23; ii++) runaction->FillHist(12, dEslice[ii]);
        for (int ii = 30; ii < 33; ii++) runaction->FillHist(13, dEslice[ii]);
        ntr = 0;
        ntr1 = 0;
        ntr2 = 0;
        ntr3 = 0;
        for (int ii = 0; ii < 100; ii++)
            if (dEslice[ii] > 5) {
                ntr++;
                if (ii < 5) ntr1++;
                if (ii < 10) ntr2++;
                if (ii < 15) ntr3++;
            }
        runaction->FillHist(20, ntr);
        runaction->FillHist(21, ntr1);
        runaction->FillHist(22, ntr2);
        runaction->FillHist(23, ntr3);

        //dedx_fadc->Write();
        //printf(" new event:%6d",nevent);  for (int ii=0;  ii<30; ii++) printf(" %4.1f",dEslice[ii]);  printf("\n");
    }


    if (save_hits_root) { //--   fill tree  ----
        const G4Event *evt = G4RunManager::GetRunManager()->GetCurrentEvent();

        const G4int primeVtxCount = evt->GetNumberOfPrimaryVertex();
        size_t particleId=0;  // prime particle ID unique for all prime vertexes

        for(G4int primeVtxIndex = 0; primeVtxIndex < primeVtxCount; primeVtxIndex++)
        {
            auto primeVtx = evt->GetPrimaryVertex(primeVtxIndex);

            // Add primary vertex to root output
            mRootEventsOut.AddPrimaryVertex(
                    (size_t) primeVtxIndex,                    /* size_t aVtxIndex, */
                    (size_t) primeVtx->GetNumberOfParticle(),  /* size_t aParticleCount, */
                    primeVtx->GetX0(),                         /* double aX, */
                    primeVtx->GetY0(),                         /* double aY, */
                    primeVtx->GetZ0(),                         /* double aZ, */
                    primeVtx->GetT0(),                         /* double aTime, */
                    primeVtx->GetWeight());                    /* double aWeight */


            const G4int partCount = primeVtx->GetNumberOfParticle();
            for(G4int partIndex = 0; partIndex < partCount; partIndex++) {
                auto particle = primeVtx->GetPrimary(partIndex);
                mRootEventsOut.AddPrimaryParticle(
                         particleId,                             /*size_t aId */
                         (size_t)primeVtxIndex,                  /*size_t aPrimeVtxId */
                         (size_t)particle->GetPDGcode(),         /*size_t aPDGCode */
                         (size_t)particle->GetTrackID(),         /*size_t aTrackId */
                         particle->GetCharge(),                  /*double aCharge */
                         particle->GetMomentumDirection().x(),   /*double aMomDirX */
                         particle->GetMomentumDirection().y(),   /*double aMomDirY */
                         particle->GetMomentumDirection().z(),   /*double aMomDirZ */
                         particle->GetTotalMomentum()/GeV,       /*double aTotalMomentum */
                         particle->GetTotalEnergy()/GeV,         /*double aTotalEnergy */
                         particle->GetProperTime()/ns,           /*double aProperTime */
                         particle->GetPolX(),                    /*double aPolX */
                         particle->GetPolY(),                    /*double aPolY */
                         particle->GetPolZ()                     /*double aPolZ */
                        );

                particleId++;
            }
        }

        mRootEventsOut.FillEvent((uint64_t)evt->GetEventID());
    }



    // Total hits/steps per event. Set it back to 0
    mHitsCount = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JLeicVertexSD::clear() {
    printf("--> JLeicVertexSD::clear() \n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void JLeicVertexSD::PrintAll() {
    printf("--> JLeicVertexSD::PrintAll() \n");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

