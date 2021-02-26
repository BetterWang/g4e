#include <stdio.h>
#include "G4UserRunAction.hh"
#include <spdlog/spdlog.h>



//=====================================================================================================

static int use_depfet = 1;
static int use_fdc = 0;

static FILE *fann; //*fopen(const char *path, const char *mode);
static int NVAR;


//=====================================================================================================

#include "CommonVertexSD.hh"

#include "CommonVertexHit.hh"
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


CommonVertexSD::CommonVertexSD(G4String name, g4e::RootOutputManager* rootOutput, ReferenceDetectorConstruction *det) :
        G4VSensitiveDetector(name),
        mDetector(det),
        mRootEventsOut(rootOutput)
{
    using namespace spdlog;
    using namespace fmt;

    debug("JLeicVertexSD()::constructor  enter");
    //fRM=G4RunManager::GetRunManager();

    NumRow = 10; // :64 depfet
    NumCol = 50; // :256 depfet

    FRAME.resize(NumRow * NumCol);
    nevent = 0;
    collectionName.insert("VTXCollection");
    HitID = new G4int[500];
    debug("--> JLeicVertexSD::Constructor({})", name);

    if (use_depfet) for (int ii = 0; ii < (NumRow * NumCol); ii++) FRAME[ii] = 0; //-- 8000;  //-- set pedestals
    if (use_fdc) {
        dedx_fadc = new TH1F("dedx_fadc", " FDC dE/dX", 50, -0.5, 49.5);
        trace(" SAVE Matrix file trd_frames.root");
        fm1 = new TFile("fdc_frames.root", "RECREATE");
        for (int ii = 0; ii < 100; ii++) dEslice[ii] = 0; // reset
    }
    //-----------------  charge sharing hist ------
    char_sh = 0;
    N_bin = 45;
    G4double Max_Sigm = 0.55;  //-- at 450 um
    if (use_depfet && char_sh) //--  diffusion
        for (int iz = 0; iz <= N_bin; iz++) {

            G4double mean1 = 0., mean2 = 0., sig1 = iz * Max_Sigm / N_bin, sig2 = iz * Max_Sigm / N_bin;



            hist_charge[iz] = new TH2F(format("hist_charge_{}", iz).c_str(),
                                       format("charge sharting sigm={}; Xpos ; Ypos;", sig1).c_str(),
                                       100, -5., +5., 100, -5., +5.); //-- 5x5 pixels

            for (int ig = 0; ig < 1000000; ig++) {
                G4double g1rnd = G4RandGauss::shoot(mean1, sig1);
                G4double g2rnd = G4RandGauss::shoot(mean2, sig2);
                hist_charge[iz]->Fill(g1rnd, g2rnd);
            }

            G4double cbin = hist_charge[iz]->GetBinContent(51, 51);
            trace("--> JLeicVertexSD::Constructor():: iz={}, sigm={} hist_charge(51,51)={} \n", iz, sig1, cbin);
        }


}

CommonVertexSD::~CommonVertexSD()
{
    delete[] HitID;
    if (use_fdc) delete dedx_fadc;
}


void CommonVertexSD::Initialize(G4HCofThisEvent *)
{
    spdlog::debug("JLeicVertexSD()::Initialize enter nevent={}\n", nevent);

    VTXCollection = new JLeicVTXHitsCollection(SensitiveDetectorName, collectionName[0]);
    for (G4int i = 0; i < 1; i++) {
        HitID[i] = -1;
    };

    nevent++;
    if (use_fdc) {
        for (int ii = 0; ii < 100; ii++) dEslice[ii] = 0; // reset
        if (!(nevent % mDetector->fModuleNumber)) { //-- number of chambers simple aproximation ----
            //      for (int ii=0; ii<100; ii++) dEslice[ii]=0; // reset
            ntr = 0;
            ntr1 = 0;
            ntr2 = 0;
            ntr3 = 0;
            //printf("reset energy \n");
        }
    }

    if (use_depfet) {
        for (int ii = 0; ii < (NumRow * NumCol); ii++) FRAME[ii] = 0;  //-- reset pedestals
    }
    if (use_fdc) {
        dedx_fadc->Reset(" ");
    }

    mHitsCount = 0;
    spdlog::debug("JLeicVertexSD()::Initialize exit");
}


G4bool CommonVertexSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    using namespace spdlog;

    spdlog::trace("--> JLeicVertexSD::ProcessHits() Enter\n");

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

    trace("xloc=%f yloc=%f zloc=%f  \n", xloc, yloc, zloc);
    if (use_fdc) {  //----- FDC / TRD  ---

        int zbin = (zloc / mm + mDetector->GetConfigRef().ci_TRD.ThicknessZ / 2.) / mDetector->fadc_slice; //-- z position , slice number
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

    G4TouchableHistory *theTouchable = (G4TouchableHistory *) (aStep->GetPreStepPoint()->GetTouchable());

    trace("--> JLeicVertexSD::ProcessHits() Vol: 0={} \n", theTouchable->GetVolume()->GetName());

  /*  Need to check how many volumes... otherwise crashes...
    trace("--> JLeicVertexSD::ProcessHits() Vol: 0={} 1={}  2={} 3={} \n",
            theTouchable->GetVolume()->GetName(),
            theTouchable->GetVolume(1)->GetName(),
            theTouchable->GetVolume(2)->GetName(),
            theTouchable->GetVolume(3)->GetName()
            );
*/
    if (use_depfet > 0) {
        G4String VTXmod = theTouchable->GetVolume()->GetName();
        trace("VTX_ladder={} \n", VTXmod.c_str());
        //   int Mod=int (VTXmod<<10);
        // printf("VTX_ladder=%d \n",Mod);
        trace("--> JLeicVertexSD::ProcessHits() de={} ADC={}  len={} Mod={}  IDxy=({},{},{})\n", edep / keV, ADC, stepl / um, VTXmod.c_str(), copyIDx_pre, copyIDy_pre,
                   copyIDz_pre);
        for (int in = 0; in < 12; in++) {
            sprintf(buffer, "VTX_ladder1_%d", in);
            if (VTXmod == buffer) {
// TODO                runaction->FillHistmatrixOccup(in, copyIDx_pre, copyIDy_pre, edep / keV);
// TODO                runaction->FillHistmatrixOccupCM(in, xstep, ystep, edep / keV);
                trace("xstep=%f ystep=%f \n", xstep, ystep);
            }
        }
    }
    //-------------------------------------------------------------------------------------
    if (char_sh > 0) {
        // TODO wtheck is that?
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


    // Save the step to root
    mRootEventsOut->SaveStep(aStep, g4e::WriteStepPointChoices::PreStepPoint, copyIDx_pre, copyIDy_pre);


    G4VPhysicalVolume *physVol = theTouchable->GetVolume();
    //theTouchable->MoveUpHistory();
    G4int JLeicNumber = 0;

    trace("--> JLeicVertexSD::ProcessHits() Vol=({}) {} Abs={} \n", physVol->GetName().c_str(), (void *) physVol, (void *) mDetector->GetAbsorberPhysicalVolume());

    if (HitID[JLeicNumber] == -1) {
        CommonVertexHit *vtxHit = new CommonVertexHit();
        if (physVol == mDetector->GetAbsorberPhysicalVolume()) vtxHit->AddAbs(edep, stepl);
        HitID[JLeicNumber] = VTXCollection->insert(vtxHit) - 1;
        if (verboseLevel > 0)
            G4cout << " New Vertex Hit on JLeic: " << JLeicNumber << G4endl;
        //printf("--> JLeicVertexSD::ProcessHits()  New Vertex Hit on JLeic: %d de=%f\n",JLeicNumber,edep/keV);
    } else {
        if (physVol == mDetector->GetAbsorberPhysicalVolume())
            (*VTXCollection)[HitID[JLeicNumber]]->AddAbs(edep, stepl);
        if (verboseLevel > 0)
            G4cout << " Energy added to JLeic: " << JLeicNumber << G4endl;
        //printf("--> JLeicVertexSD::ProcessHits()  Energy added to JLeic: %d  de=%f\n",JLeicNumber,edep/keV);
    }

    trace("--> JLeicVertexSD::ProcessHits() Exit");

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void CommonVertexSD::EndOfEvent(G4HCofThisEvent *HCE)
{

    static G4int HCID = -1;
    if (HCID < 0) { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
    HCE->AddHitsCollection(HCID, VTXCollection);
    //printf("--> JLeicVertexSD::EndOfEvent() \n");

    if (use_fdc) {

        if (NVAR > mDetector->GetConfigRef().ci_TRD.ThicknessZ / mDetector->fadc_slice) {
            printf("Error FADC slices:: NVAR=%d Slices=%f \n", NVAR, mDetector->GetConfigRef().ci_TRD.ThicknessZ / mDetector->fadc_slice);
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
            // TODO runaction->FillHist2d(1, dEslice[ii], (double) ii, 1.);
            // TODO runaction->FillHLikelihood(ii, dEslice[ii]);
        }
        // TODO runaction->FillHist(10, etot);
        // TODO for (int ii = 0; ii < 3; ii++) runaction->FillHist(11, dEslice[ii]);
        // TODO for (int ii = 20; ii < 23; ii++) runaction->FillHist(12, dEslice[ii]);
        // TODO for (int ii = 30; ii < 33; ii++) runaction->FillHist(13, dEslice[ii]);
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
        // TODO runaction->FillHist(20, ntr);
        // TODO runaction->FillHist(21, ntr1);
        // TODO runaction->FillHist(22, ntr2);
        // TODO runaction->FillHist(23, ntr3);

        //dedx_fadc->Write();
        //printf(" new event:%6d",nevent);  for (int ii=0;  ii<30; ii++) printf(" %4.1f",dEslice[ii]);  printf("\n");
    }
    // Total hits/steps per event. Set it back to 0
    mHitsCount = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void CommonVertexSD::clear()
{
    printf("--> JLeicVertexSD::clear() \n");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void CommonVertexSD::PrintAll()
{
    printf("--> JLeicVertexSD::PrintAll() \n");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

