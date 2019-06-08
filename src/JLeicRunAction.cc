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
// JLeicRunAction.cc,
// 
//
// 


#include <string>

#include "JLeicRunAction.hh"
#include "JLeicRunMessenger.hh"
#include "JLeicDetectorConstruction.hh"
#include "JLeicPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include <iomanip>

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

#include "rootlib.h"

static TH1D *histo1,*histo2,*histo3,*histo4,*histo5,*histo6,*histo7,*histo8,*histo9,*histo10,*histo11,*histo12,*histo13,*histo14;
//static TH2D* d2_pos_ffq1;
const int NHIST=50;
static TH1D* hist[NHIST];
static TH1D* HLikelihood[NHIST];
static char Hname[256];
static TH2D* d2_hist[NHIST];
static char RootFileName[256];
static char FileName[256];
static int NumRow=10;
static int NumCol=50;
static TH2F *hmatrixOccup[120];
static TH2F *hmatrixOccupCM[120];
 
//////////////////////////////////////////////////////////////////////////////

JLeicRunAction::JLeicRunAction(JLeicDetectorConstruction* DET):
        histName("histfile"),
        detector (DET),
        nbinStep(0),
        nbinEn(0),
        nbinTt(0),
        nbinTb(0),
        nbinTsec(0),
        nbinTh(0),
        nbinThback(0),
        nbinR(0),
        nbinGamma(0),
        nbinvertexz(0)
{
  runMessenger = new JLeicRunMessenger(this);
  saveRndm = 1;

}

////////////////////////////////////////////////////////////////////////////

JLeicRunAction::~JLeicRunAction()
{
  delete runMessenger;
}

////////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::bookHisto()
{
  char myname[120];
  printf("Setup=%s \n",detector->fSetUp.c_str());
  G4cout << "Setup=" << detector->fSetUp;

  printf("------------------- > Booking histograms ...  \n");
  
  // init hbook

  //  hbookManager = new HBookFile(histName, 68);
  //  assert (hbookManager != 0);

  // book histograms

  if(nbinStep>0)
  {
    printf("book histo1 \n");
    histo1= new TH1D("histo1","number of steps/event",nbinStep, Steplow,Stephigh);
    //histo1 = hbookManager->histogram("number of steps/event",nbinStep,Steplow,Stephigh) ;
    //assert (histo1 != 0);
  }
  if(nbinEn>0)
  {
    printf("book histo2 \n");
    histo2= new TH1D("histo2","Energy Loss (keV)",nbinEn,Enlow/keV,Enhigh/keV);
    //histo2 = hbookManager->histogram("Energy Loss (keV)",nbinEn,Enlow/keV,Enhigh/keV) ;
    //assert (histo2 != 0);
  }
  if(nbinTh>0)
  {
    histo3= new TH1D("histo3","angle distribution at exit(deg)",nbinTh,Thlow/deg,Thhigh/deg);
    //histo3 = hbookManager->histogram("angle distribution at exit(deg)",nbinTh,Thlow/deg,Thhigh/deg) ;
    //assert (histo3 != 0);
  }
  if(nbinR>0)
  {
    histo4= new TH1D("histo4","lateral distribution at exit(mm)",nbinR ,Rlow,Rhigh);
    //histo4 = hbookManager->histogram("lateral distribution at exit(mm)",nbinR ,Rlow,Rhigh)  ;
    //assert (histo4 != 0);
  }
  if(nbinTt>0)
  {
    histo5= new TH1D("histo5","kinetic energy of the primary at exit(MeV)",nbinTt,Ttlow,Tthigh);
    //histo5 = hbookManager->histogram("kinetic energy of the primary at exit(MeV)",nbinTt,Ttlow,Tthigh)  ;
    //assert (histo5 != 0);
  }
  if(nbinThback>0)
  {
    histo6= new TH1D("histo6","angle distribution of backscattered primaries(deg)",nbinThback,Thlowback/deg,Thhighback/deg);
    //histo6 = hbookManager->histogram("angle distribution of backscattered primaries(deg)",nbinThback,Thlowback/deg,Thhighback/deg) ;
    //assert (histo6 != 0);
  }
  if(nbinTb>0)
  {
    histo7= new TH1D("histo7","kinetic energy of the backscattered primaries (MeV)",nbinTb,Tblow,Tbhigh);
    //histo7 = hbookManager->histogram("kinetic energy of the backscattered primaries (MeV)",nbinTb,Tblow,Tbhigh)  ;
    //assert (histo7 != 0);
  }
  if(nbinTsec>0)
  {
    histo8= new TH1D("histo8","kinetic energy of the charged secondaries (keV)",nbinTsec,Tseclow,Tsechigh);
    //histo8 = hbookManager->histogram("kinetic energy of the charged secondaries (MeV)",nbinTsec,Tseclow,Tsechigh)  ;
    //assert (histo8 != 0);
  }
  if(nbinvertexz>0)
  {
    histo9= new TH1D("histo9","z of secondary charged vertices(mm)",nbinvertexz ,zlow,zhigh);
    //histo9 = hbookManager->histogram("z of secondary charged vertices(mm)",nbinvertexz ,zlow,zhigh)  ;
    //assert (histo9 != 0);
  }
  //  if(nbinGamma>0)
  //  {
  //printf("book histo10 \n");
  // histo10= new TH1D("histo10","kinetic energy of gammas escaping the absorber (keV)",nbinGamma,0.,50.)  ;
		      //		      ,nbinGamma,std::log10(ElowGamma),std::log10(EhighGamma));
    //histo10= hbookManager->histogram("kinetic energy of gammas escaping the absorber (MeV)"
                                //     ,nbinGamma,ElowGamma,EhighGamma)  ;
    //                            ,nbinGamma,std::log10(ElowGamma),std::log10(EhighGamma))  ;
    //assert (histo10 != 0);
    //  }
  printf("book  nbin=%d Elow=%f Ehigh=%f\n",nbinGamma,ElowGamma,EhighGamma);

  int nb=500;
  float emin=0, emax=50.;
  histo10= new TH1D("histo10","kinetic energy of gammas escaping the absorber (keV)",nb,emin,emax)  ;
  histo11= new TH1D("histo11","dE of gammas in the absorber (keV) ",nb ,emin,emax);   //-- keV
  histo12= new TH1D("histo12","kinetic energy of gammas entering the absorber (keV) ",nb ,emin,emax);   //-- keV
  histo14= new TH1D("histo14","kinetic energy of gammas in step (keV) ",nb ,emin,emax);   //-- keV


  histo13= new TH1D("histo13","gamma angle (mrad) ",500 ,0.,12.);   //-- mrad

  hist[1]= new TH1D("hist1","N Photon Absorption vs Z (mm)",100, 0., 50.);
  hist[2]= new TH1D("hist2","E Photon Absorption vs Z (mm)",100, 0., 50.);
  hist[3]= new TH1D("hist3","Step length charged, um ",100, 0., 1000.);
  hist[4]= new TH1D("hist4","dE/dx sec vs Z  mm",100, 0., 50.);
  hist[5]= new TH1D("hist5","dE/dx prim vs Z mm",100, 0., 50.);
  hist[6]= new TH1D("hist6","dE/dx charged vs Z mm",100, 0., 50.);
  hist[7]= new TH1D("hist7","dE/dx vs Z mm",100, 0., 50.);

  hist[10]= new TH1D("hist10","dE/dx total (keV) ",200 ,0.,200);   //-- keV
  nb=100;  emin=0; emax=20.;
  hist[11]= new TH1D("hist11","dE/dx slice1 (keV) ",nb ,emin,emax);   //-- keV
  hist[12]= new TH1D("hist12","dE/dx slice2 (keV) ",nb ,emin,emax);   //-- keV
  hist[13]= new TH1D("hist13","dE/dx slice3 (keV) ",nb ,emin,emax);   //-- keV
  for (int ii=10; ii<=13; ii++)   hist[ii]->GetXaxis()->SetTitle("Energy , keV");

  hist[20]= new TH1D("hist20"," N TR photons bin:1-100",50 ,-0.5,49.5);  hist[20]->GetXaxis()->SetTitle("Number TR");
  hist[21]= new TH1D("hist21"," N TR photons bin:1-5 ",50 ,-0.5,49.5);  hist[21]->GetXaxis()->SetTitle("Number TR");
  hist[22]= new TH1D("hist22"," N TR photons bin:1-10",50 ,-0.5,49.5);  hist[22]->GetXaxis()->SetTitle("Number TR");
  hist[23]= new TH1D("hist23"," N TR photons bin:1-15",50 ,-0.5,49.5);  hist[23]->GetXaxis()->SetTitle("Number TR");

 hist[25]= new TH1D("hist25","dE/dx charged  PIXEL",1000, 0., 1000.);
 hist[26]= new TH1D("hist26","dE/dx charged STRIP ",1000, 0., 1000.);
 hist[27]= new TH1D("hist27","Energy deposit gamma",1000, 0., 1000.);


  d2_hist[1] = new TH2D("Hist2d1","dE/dx spectrum in slices ",50,emin,emax,40,-0.5,39.5); 
  d2_hist[2] = new TH2D("Hist2d2","dE/dx  vs energy PIXEL ",1000, 0., 1000.,1000, 0., 1000.); 
  d2_hist[3] = new TH2D("Hist2d3","dE/dx  vs energy STRIP",1000, 0., 1000.,1000, 0., 5000.); 
 
  // ---- position at the  entrance of dipole -----
  d2_hist[4] = new TH2D("d2_pos_iBDS1a_in"," d2_pos_iBDS1a_in ",100,-1500.,1000.,100,-20.,20.); 
  d2_hist[5] = new TH2D("d2_pos_iBDS1b_in"," d2_pos_iBDS1b_in ",100,-1500.,0.,100,-20.,20.); 
  d2_hist[6] = new TH2D("d2_pos_iBDS2_in"," d2_pos_iBDS2_in ",100,-3500.,3500.,100,-20.,20.); 
  d2_hist[7] = new TH2D("d2_pos_iBDS3_in"," d2_pos_iBDS3_in ",100,-3500.,3500.,100,-20.,20.); 

  // ---- position at the entrance of quadr -----

  d2_hist[8] = new TH2D("d2_pos_iQDS1a_in"," d2_pos_iQDS1a_in ",100,-500.,0.,100,-20.,20.); 
  d2_hist[9] = new TH2D("d2_pos_iQDS1S_in"," d2_pos_iQDS1S_in ",100,-1500.,0.,100,-20.,20.); 
  d2_hist[10] = new TH2D("d2_pos_iQDS1b_in"," d2_pos_iQDS1b_in ",100,-1500.,0.,100,-20.,20.); 
  d2_hist[11] = new TH2D("d2_pos_iQDS2S_in"," d2_pos_iQDS2S_in ",100,-1500.,0.,100,-20.,20.); 
  d2_hist[12] = new TH2D("d2_pos_iQDS2_in"," d2_pos_iQDS2_in ",100,-1500.,0.,100,-20.,20.); 
  d2_hist[13] = new TH2D("d2_pos_iQDS3S_in"," d2_pos_iQDS3S_in ",100,-1500.,0.,100,-20.,20.); 

for (int in=0;in<12;in++) {    
    sprintf(myname,"hmatrixOccup1_%d",in);
    hmatrixOccup[in]= new TH2F(myname,myname,NumCol,-0.5,NumCol-0.5,NumRow,-0.5,NumRow-0.5);
    sprintf(myname,"hmatrixOccupCM1_%d",in);
    hmatrixOccupCM[in]= new TH2F(myname,myname,100,-10.,10.,100,-10.,10.);
  }

  for (int is=0; is<detector->NannVAR; is++) {
    if (is<NHIST) {
      sprintf(Hname,"Likelihood:%02d",is);
      HLikelihood[is]=new TH1D(Hname,Hname,100,emin,emax*(1+detector->fadc_slice/2.));
    }
  }


  gStyle->SetOptStat("nemruo");
    printf("booking done ! \n");
  
}


//////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillHLikelihood(int ihist, G4double val)
{

  //printf("FillHLikelihood(%d, %f)  ptr=%p\n",ihist,val,HLikelihood[ihist]);
  if( ihist<NHIST && HLikelihood[ihist]) HLikelihood[ihist]->Fill(val) ; 

}
void JLeicRunAction::FillHist(int ihist, G4double val)
{

 if(hist[ihist]) hist[ihist]->Fill(val) ; 

}
void JLeicRunAction::FillHist(int ihist, G4double val, G4double w)
{

  if(hist[ihist]) hist[ihist]->Fill(val,w) ; 

}
void JLeicRunAction::FillHist(int ihist, int bin, G4double w)
{

  if(hist[ihist]) hist[ihist]->Fill(bin,w) ; 

}
void JLeicRunAction::ResetHist(int ihist)
{

  if(hist[ihist]) hist[ihist]->Reset() ; 

}
//---  2D ---
void JLeicRunAction::FillHist2d(int ihist, G4double valx, G4double valy, G4double w)
{

  if(d2_hist[ihist]) d2_hist[ihist]->Fill(valx,valy,w) ; 

}
G4double JLeicRunAction::GetHist2d(int ihist, G4int ix,G4int iy) 
{

  if(d2_hist[ihist]) return d2_hist[ihist]->GetBinContent(ix, iy);

}

void JLeicRunAction::ResetHist2d(int ihist)
{

  if(d2_hist[ihist]) d2_hist[ihist]->Reset() ; 

}
void JLeicRunAction::FillHistmatrixOccup(int ihist, G4double valx, G4double valy, G4double w)
{

  // printf("ihist= %d\n",ihist);
  //  if( hmatrixOccup[ihist])  hmatrixOccup[ihist]->Fill(valx,valy,w) ; 
  if( hmatrixOccup[ihist])  hmatrixOccup[ihist]->Fill(valx,valy,1.) ; 

}
void JLeicRunAction::FillHistmatrixOccupCM(int ihist, G4double valx, G4double valy, G4double w)
{

  // printf("ihist= %d\n",ihist);
  //  if( hmatrixOccup[ihist])  hmatrixOccup[ihist]->Fill(valx,valy,w) ; 
  if( hmatrixOccupCM[ihist])  hmatrixOccupCM[ihist]->Fill(valx,valy,1.) ; 

}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::BeginOfRunAction(const G4Run* aRun)
{  
  printf("RunAction:: Enter\n");
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  // save Rndm status
  if (saveRndm > 0)
  { 
      CLHEP::HepRandom::showEngineStatus();
      CLHEP::HepRandom::saveEngineStatus("beginOfRun.rndm");
  }  
  G4UImanager* UI = G4UImanager::GetUIpointer();
   
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager)    UI->ApplyCommand("/vis/scene/notifyHandlers");

  printf("RunAction:: 1\n");
      
  EnergySumAbs = 0. ;
  EnergySquareSumAbs = 0.;
  tlSumAbs = 0. ;
  tlsquareSumAbs = 0. ;
  nStepSumCharged = 0. ;
  nStepSum2Charged= 0. ;
  nStepSumNeutral = 0. ;
  nStepSum2Neutral= 0. ;
  TotNbofEvents = 0. ;
  SumCharged=0.;
  SumNeutral=0.;
  Sum2Charged=0.;
  Sum2Neutral=0.;
  Selectron=0.;
  Spositron=0.;

  Transmitted=0.;
  Reflected  =0.;

//  plot definitions 
  printf("RunAction:: 2\n");
   
  if(nbinStep>0)
  {
    dStep=(Stephigh-Steplow)/nbinStep;
    entryStep=0.;
    underStep=0.;
    overStep=0.;
    for(G4int ist=0; ist<nbinStep; ist++)
    {
      distStep[ist]=0.;
    }
  }      
  if(nbinEn>0)
  {
    dEn = (Enhigh-Enlow)/nbinEn ;
    entryEn=0.;
    underEn=0.;
    overEn=0.;

    for (G4int ien=0; ien<nbinEn; ien++)   distEn[ien]=0.;
  }
  if(nbinTt>0)
  {
    dTt = (Tthigh-Ttlow)/nbinTt ;
    entryTt=0.;
    underTt=0.;
    overTt=0.;

    printf("RunAction:: 3\n");

    for (G4int itt=0; itt<nbinTt; itt++)  distTt[itt]=0.;

    Ttmean=0.;
    Tt2mean=0.;
  }
  if(nbinTb>0)
  {
    dTb = (Tbhigh-Tblow)/nbinTb ;
    entryTb=0.;
    underTb=0.;
    overTb=0.;
    for (G4int itt=0; itt<nbinTb; itt++)
    {
      distTb[itt]=0.;
    }
    Tbmean=0.;
    Tb2mean=0.;
  }
  if(nbinTsec>0)
  {
    dTsec = (Tsechigh-Tseclow)/nbinTsec ;
    entryTsec=0.;
    underTsec=0.;
    overTsec=0.;
    for (G4int its=0; its<nbinTsec; its++)
    {
      distTsec[its]=0.;
    }
  }
  if(nbinTh>0)
  {
    dTh = (Thhigh-Thlow)/nbinTh ;
    entryTh=0.;
    underTh=0.;
    overTh=0.;
    for (G4int ith=0; ith<nbinTh; ith++)
    {
      distTh[ith]=0.;
    }
  }
    printf("RunAction:: 4\n");

  if(nbinThback>0)
  {
    dThback = (Thhighback-Thlowback)/nbinThback ;
    entryThback=0.;
    underThback=0.;
    overThback=0.;
    for (G4int ithback=0; ithback<nbinThback; ithback++)
    {
      distThback[ithback]=0.;
    }
  }


  if(nbinR >0)
  {
    dR  = (Rhigh-Rlow)/nbinR  ;
    entryR =0.;
    underR =0.;
    overR =0.;
    for (G4int ir =0; ir <nbinR ; ir++)
    {
      distR[ir]=0.;
    }
    Rmean=0.;
    R2mean=0.;
  }
    printf("RunAction:: 5\n");
    /*
  if(nbinGamma>0)
  {
    dEGamma = std::log(EhighGamma/ElowGamma)/nbinGamma ;
    entryGamma = 0.;
    underGamma=0.;
    overGamma=0.;
    for (G4int ig=0; ig<nbinGamma; ig++)
    {
      distGamma[ig]=0.;
    }
  } 
    */
    printf("RunAction:: 6\n");

  if(nbinvertexz>0)
  {
    dz=(zhigh-zlow)/nbinvertexz;
    entryvertexz=0.;
    undervertexz=0.;
    oververtexz=0.;
    for(G4int iz=0; iz<nbinvertexz; iz++)
    {
      distvertexz[iz]=0.;
    }
  }
    printf("RunAction:: 9\n");

  bookHisto();

    printf("RunAction:: exit\n");


}
  //thePrimaryParticleName = particleGun->GetParticleDefinition()->GetParticleName();
 
void JLeicRunAction::SetName() {

  sprintf(FileName,"%s_%s%.fGeV_%s_d%.f:%.1fmm_r%.fcm_m%d"
	  ,detector->fSetUp.c_str() 
	  ,JLeicPrimaryGeneratorAction::GetPrimaryName().c_str()
	  ,JLeicPrimaryGeneratorAction::GetPrimaryEnergy()/GeV
	  //,particleGun->GetParticleDefinition()->GetParticleName()
	  ,detector->fAbsorberMaterial->GetName().c_str() 
	  ,detector->fAbsorberThickness
	  ,detector->fadc_slice
	  ,detector->fRadThick/cm
	  ,detector->fModuleNumber
	  );

  sprintf(RootFileName,"%s.root",FileName);
  //AnnFileName = string("ann_")+string(FileName);
  AnnFileName = "aaa";
  printf("RunAction:: \nFile Name = %s \nRoot=%s  \nAnn=%s \n",FileName,RootFileName,AnnFileName.c_str());

}
/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::EndOfRunAction(const G4Run*)
{
  G4double sAbs,sigAbs,sigstep,sigcharged,signeutral;


  tlSumAbs /= TotNbofEvents ;
  sAbs = tlsquareSumAbs/TotNbofEvents-tlSumAbs*tlSumAbs ;
  if(sAbs>0.)
    sAbs = std::sqrt(sAbs/TotNbofEvents) ;
  else
    sAbs = 0. ;
  
  EnergySumAbs /= TotNbofEvents ;
  sigAbs = EnergySquareSumAbs/TotNbofEvents-EnergySumAbs*EnergySumAbs;
  if(sigAbs>0.)
    sigAbs = std::sqrt(sigAbs/TotNbofEvents);
  else
    sigAbs = 0.;

  nStepSumCharged /= TotNbofEvents ;
  sigstep = nStepSum2Charged/TotNbofEvents-nStepSumCharged*nStepSumCharged;
  if(sigstep>0.)
    sigstep = std::sqrt(sigstep/TotNbofEvents);
  else
    sigstep = 0.;
  G4double sigch=sigstep ;
  
  nStepSumNeutral /= TotNbofEvents ;
  sigstep = nStepSum2Neutral/TotNbofEvents-nStepSumNeutral*nStepSumNeutral;
  if(sigstep>0.)
    sigstep = std::sqrt(sigstep/TotNbofEvents);
  else
    sigstep = 0.;
  G4double signe=sigstep ;
  
  SumCharged /= TotNbofEvents;
  sigcharged = Sum2Charged/TotNbofEvents-SumCharged*SumCharged; 
  if(sigcharged>0.)
    sigcharged = std::sqrt(sigcharged/TotNbofEvents);
  else
    sigcharged = 0. ;
 
  SumNeutral /= TotNbofEvents;
  signeutral = Sum2Neutral/TotNbofEvents-SumNeutral*SumNeutral; 
  if(signeutral>0.)
    signeutral = std::sqrt(signeutral/TotNbofEvents);
  else
    signeutral = 0. ;
 
  Selectron /= TotNbofEvents ;
  Spositron /= TotNbofEvents ;

  Transmitted /=TotNbofEvents ;
  Reflected   /=TotNbofEvents ;
 G4cout << " ================== run summary =====================" << G4endl;
 G4int prec = G4cout.precision(6);
  G4cout << " end of Run TotNbofEvents = " <<  
           TotNbofEvents << G4endl ;
  G4cout << "    mean charged track length   in absorber=" <<
           tlSumAbs/mm      << " +- " << sAbs/mm    <<
          "  mm  " << G4endl; 
  G4cout << G4endl;
  G4cout << "            mean energy deposit in absorber=" <<
           EnergySumAbs/MeV << " +- " << sigAbs/MeV <<
          "  MeV " << G4endl ;
  G4cout << G4endl ;
  G4cout << " mean number of steps in absorber (charged) =" <<
           nStepSumCharged         << " +- " << sigch     <<
          "      " << G4endl ;
  G4cout << " mean number of steps in absorber (neutral) =" <<
           nStepSumNeutral         << " +- " << signe     <<
          "      " << G4endl ;
  G4cout << G4endl ;
  G4cout << "   mean number of charged secondaries = " <<
           SumCharged << " +- " << sigcharged << G4endl;  
  G4cout << G4endl ;
  G4cout << "   mean number of neutral secondaries = " <<
           SumNeutral << " +- " << signeutral << G4endl;  
  G4cout << G4endl ;
  
  G4cout << "   mean number of e-s =" << Selectron << 
            "  and e+s =" << Spositron << G4endl;
  G4cout << G4endl; 
  
  G4cout << "(number) transmission coeff=" << Transmitted <<
            "  reflection coeff=" << Reflected << G4endl;
  G4cout << G4endl; 
  /*
  if(nbinStep>0)
  {G4double E , dnorm, norm ;
   G4cout << "   step number/event distribution " << G4endl ;
   G4cout << "#entries=" << entryStep << "    #underflows=" << underStep <<
             "    #overflows=" << overStep << G4endl ;
   if( entryStep>0.)
   {
     E = Steplow - dStep ;
     norm = TotNbofEvents ;
     G4cout << " bin nb   nsteplow     entries     normalized " << G4endl ;
     for(G4int iss=0; iss<nbinStep; iss++)
     {
      E += dStep ;
      dnorm = distStep[iss]/norm;
      G4cout << std::setw(5) << iss << std::setw(10) << E << 
                std::setw(12) << distStep[iss] <<
                std::setw(12) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }     
  }
  if(nbinEn > 0)
  {
    std::ofstream fileOut("distribution.out", std::ios::out ) ;
    fileOut.setf( std::ios::scientific, std::ios::floatfield );

    std::ofstream normOut("normDist.out", std::ios::out ) ;
    normOut.setf( std::ios::scientific, std::ios::floatfield );

    G4double E , dnorm, norm,fmax,Emp ;
    Emp=-999.999 ;
    G4cout << " energy deposit distribution " << G4endl ;
    G4cout << "#entries=" << entryEn << "    #underflows=" << underEn <<
             "    #overflows=" << overEn << G4endl ;
    if( entryEn>0.)
    {
      E = Enlow - dEn ;
      norm = TotNbofEvents*1.0 ;   // *dEn ;
      G4cout << " bin nb      Elow      entries     normalized " << G4endl ;
      fmax = 0. ;

      for(G4int ien=0; ien<nbinEn; ien++)
      {
        E += dEn ;

        if(distEn[ien]>fmax)
        {
          fmax = distEn[ien] ;
          Emp = E ;                // most probable roughly
        }
        dnorm = distEn[ien]/norm;

        G4cout << std::setw(5) << ien << std::setw(10) << E/keV << 
                  std::setw(12) << distEn[ien] <<
                  std::setw(12) << dnorm << G4endl ;

        fileOut << E/keV << "\t"<< distEn[ien] << G4endl ;
        normOut << E/keV << "\t"<< dnorm << G4endl ;
      }
      G4cout << G4endl;
      G4int ii ;
      G4double E1,E2 ;
      E1=-1.e6 ;
      E2=+1.e6 ;
      E = Enlow -dEn ;
      ii = -1;

      for(G4int i1=0; i1<nbinEn; i1++)
      {
        E += dEn ;
        if(ii<0)
        {
          if(distEn[i1] >= 0.5*fmax)
          {
            E1=E ;
            ii=i1 ;
          }
        }
      }
      E = Enlow -dEn ;

      for(G4int i2=0; i2<nbinEn; i2++)
      {
        E += dEn ;

        if(distEn[i2] >= 0.5*fmax)   E2=E ;
      }
      G4cout << " Emp = " << std::setw(15) << Emp/MeV << "   width="
            << std::setw(15) << (E2-E1)/MeV <<   "  MeV " << G4endl;
      G4cout << G4endl ;
    }     
  }
  if(nbinTt>0)
  {
     G4double E , dnorm, norm ,sig;
   G4cout << " transmitted energy distribution " << G4endl ;
   G4cout << "#entries=" << entryTt << "    #underflows=" << underTt <<
             "    #overflows=" << overTt << G4endl ;
   if( entryTt>0.)
   {
     Ttmean /= entryTt;
     sig=Tt2mean/entryTt-Ttmean*Ttmean ;
     if(sig<=0.)
       sig=0.;
     else
       sig=std::sqrt(sig/entryTt) ;
     G4cout << " mean energy of transmitted particles=" << Ttmean/keV << 
               " +- " << sig/keV << "  keV." << G4endl;
     E = Ttlow - dTt ;
     norm = TotNbofEvents*dTt ;
     G4cout << " bin nb      Elow      entries     normalized " << G4endl ;
     for(G4int itt=0; itt<nbinTt; itt++)
     {
      E += dTt ;
      dnorm = distTt[itt]/norm;
      G4cout << std::setw(5) << itt << std::setw(10) << E << 
                std::setw(12) << distTt[itt] <<
                std::setw(12) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }     
  }
  if(nbinTb>0)
  {
     G4double E , dnorm, norm ,sig;
   G4cout << " backscattered energy distribution " << G4endl ;
   G4cout << "#entries=" << entryTb << "    #underflows=" << underTb <<
             "    #overflows=" << overTb << G4endl ;
   if( entryTb>0.)
   {
     Tbmean /= entryTb;
     sig=Tb2mean/entryTb-Tbmean*Tbmean ;
     if(sig<=0.)
       sig=0.;
     else
       sig=std::sqrt(sig/entryTb) ;
     G4cout << " mean energy of backscattered particles=" << Tbmean/keV << 
               " +- " << sig/keV << "  keV." << G4endl;
     E = Tblow - dTb ;
     norm = TotNbofEvents*dTb ;
     G4cout << " bin nb      Elow      entries     normalized " << G4endl ;
     for(G4int itt=0; itt<nbinTb; itt++)
     {
      E += dTb ;
      dnorm = distTb[itt]/norm;
      G4cout << std::setw(5) << itt << std::setw(10) << E << 
                std::setw(12) << distTb[itt] <<
                std::setw(12) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }     
  }
  if(nbinTsec>0)
  {G4double E , dnorm, norm ;
   G4cout << " energy distribution of charged secondaries " << G4endl ;
   G4cout << "#entries=" << entryTsec << "    #underflows=" << underTsec <<
             "    #overflows=" << overTsec << G4endl ;
   if( entryTsec>0.)
   {
     E = Tseclow - dTsec ;
     norm = TotNbofEvents*dTsec ;
     G4cout << " bin nb      Elow      entries     normalized " << G4endl ;
     for(G4int itt=0; itt<nbinTsec; itt++)
     {
      E += dTsec ;
      dnorm = distTsec[itt]/norm;
      G4cout << std::setw(5) << itt << std::setw(10) << E << 
                std::setw(12) << distTsec[itt] <<
                std::setw(12) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }     
  }

  if(nbinR >0)
  {G4double R , dnorm, norm,sig  ;
   G4cout << "  R  distribution " << G4endl ;
   G4cout << "#entries=" << entryR  << "    #underflows=" << underR  <<
             "    #overflows=" << overR  << G4endl ;
   if( entryR >0.)
   {
     Rmean /= entryR;
     sig = R2mean/entryR - Rmean*Rmean;
     if(sig<=0.) sig=0. ;
     else        sig = std::sqrt(sig/entryR) ;
     G4cout << " mean lateral displacement at exit=" << Rmean/mm << " +- "
            << sig/mm << "  mm." << G4endl ; 
     R = Rlow - dR  ;
     norm = TotNbofEvents*dR  ;
     G4cout << " bin nb      Rlow      entries     normalized " << G4endl ;
     for(G4int ier=0; ier<nbinR ; ier++)
     {
      R+= dR  ;
      dnorm = distR[ier]/norm;
      G4cout << std::setw(5) << ier << std::setw(10) << R  <<
                std::setw(12) << distR[ier] <<
                std::setw(12) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }
  }

  if(nbinTh>0)
  {G4double Th,Thdeg, dnorm, norm,fac0,fnorm,pere,Thpere,Thmean,sum;
   G4cout << "      angle   distribution " << G4endl ;
   G4cout << "#entries=" << entryTh << "    #underflows=" << underTh <<
             "    #overflows=" << overTh << G4endl ;
   if( entryTh>0.)
   {
     Th= Thlow - dTh ;
     norm = TotNbofEvents ;
     if(distTh[0] == 0.)
       fac0 = 1. ;
     else
       fac0 = 1./distTh[0] ;
     pere = 1./std::exp(1.) ;

     G4cout << " bin nb  Thlowdeg      Thlowrad      " <<
               " entries         normalized " << G4endl ;
     Thpere = 0. ;
     sum = 0. ;
     Thmean = 0. ;
     for(G4int ien=0; ien<nbinTh; ien++)
     {
      Th+= dTh ;
      Thdeg = Th*180./pi ;
      sum += distTh[ien] ;
      Thmean += distTh[ien]*(Th+0.5*dTh) ;
      dnorm = distTh[ien]/norm;
      fnorm = fac0*distTh[ien] ;
      if( fnorm > pere)
        Thpere = Th ; 
      G4cout << std::setw(5) << ien << std::setw(10) << Thdeg << "   " <<
                std::setw(10) << Th << "  " <<   
                std::setw(12) << distTh[ien] << "  " <<
                std::setw(12) << dnorm << "  " << std::setw(12) << fnorm <<G4endl ;
     }
     Thmean /= sum ;
     G4cout << G4endl;
     G4cout << " mean = " << Thmean << "  rad  or " << 180.*Thmean/pi <<
               " deg." << G4endl;
     G4cout << " theta(1/e)=" << Thpere << " - " << Thpere+dTh << " rad   "
            << " or " << 180.*Thpere/pi << " - " << 180.*(Thpere+dTh)/pi 
            << " deg." << G4endl;
     G4cout << G4endl;
   }
  }

  if(nbinThback>0)
  {G4double Thb,Thdegb, dnormb, normb,fac0b,fnormb,pereb,Thpereb,Thmeanb,sumb;
   G4cout << " backscattering angle   distribution " << G4endl ;
   G4cout << "#entries=" << entryThback << "    #underflows=" << underThback <<
             "    #overflows=" << overThback << G4endl ;
   if( entryThback>0.)
   {
     Thb= Thlowback - dThback ;
     normb = TotNbofEvents ;
     if(distThback[0] == 0.)
       fac0b = 1. ;
     else
       fac0b = 1./distThback[0] ;
     pereb = 1./std::exp(1.) ;

     G4cout << " bin nb  Thlowdeg      Thlowrad      " <<
               " entries         normalized " << G4endl ;
     Thpereb = 0. ;
     sumb = 0. ;
     Thmeanb = 0. ;
     for(G4int ien=0; ien<nbinThback; ien++)
     {
      Thb+= dThback ;
      Thdegb = Thb*180./pi ;
      sumb += distThback[ien] ;
      Thmeanb += distThback[ien]*(Thb+0.5*dThback) ;
      dnormb = distThback[ien]/normb;
      fnormb = fac0b*distThback[ien] ;
      if( fnormb > pereb)
        Thpereb = Thb ;
      G4cout << std::setw(5) << ien << std::setw(10) << Thdegb << "   " <<
                std::setw(10) << Thb << "  " <<
                std::setw(12) << distThback[ien] << "  " <<
                std::setw(12) << dnormb << "  " << std::setw(12) << fnormb <<G4endl ;
     }
     Thmeanb /= sumb ;
     G4cout << G4endl;
     G4cout << " mean = " << Thmeanb << "  rad  or " << 180.*Thmeanb/pi <<
               " deg." << G4endl;
     G4cout << " theta(1/e)=" << Thpereb << " - " << Thpereb+dThback << " rad   "
            << " or " << 180.*Thpereb/pi << " - " << 180.*(Thpereb+dThback)/pi
            << " deg." << G4endl;
     G4cout << G4endl;
   }
  }

  if(nbinGamma>0)
  {G4double E , fact,dnorm, norm  ;
   G4cout << " gamma energy distribution " << G4endl ;
   G4cout << "#entries=" << entryGamma << "    #underflows=" << underGamma <<
             "    #overflows=" << overGamma << G4endl ;
   if( entryGamma>0.)
   {
     fact=std::exp(dEGamma) ;
     E = ElowGamma/fact  ;
     norm = TotNbofEvents*dEGamma;
     G4cout << " bin nb         Elow      entries       normalized " << G4endl ;
     for(G4int itt=0; itt<nbinGamma; itt++)
     {
      E *= fact ;
      dnorm = distGamma[itt]/norm;
      G4cout << std::setw(5) << itt << std::setw(13) << E << 
                std::setw(12) << distGamma[itt] <<
                std::setw(15) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }     
  }

  if(nbinvertexz >0)
  {G4double z , dnorm, norm  ;
   G4cout << " vertex Z  distribution " << G4endl ;
   G4cout << "#entries=" << entryvertexz  << "    #underflows=" << undervertexz  <<
             "    #overflows=" << oververtexz  << G4endl ;
   if( entryvertexz >0.)
   {
     z =zlow - dz  ;
     norm = TotNbofEvents*dz  ;
     G4cout << " bin nb      zlow      entries     normalized " << G4endl ;
     for(G4int iez=0; iez<nbinvertexz ; iez++)
     {
      z+= dz  ;
      if(std::fabs(z)<1.e-12) z=0.;
      dnorm = distvertexz[iez]/norm;
      G4cout << std::setw(5) << iez << std::setw(10) << z  <<
                std::setw(12) << distvertexz[iez] <<
                std::setw(12) << dnorm << G4endl ;
     }
     G4cout << G4endl;
   }
  }
  */
 G4cout.precision(prec);

 printf(" ... viwer/update\n");  

  if (G4VVisManager::GetConcreteInstance())
  {
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }

  // save Rndm status

  if (saveRndm == 1)
  { 
    CLHEP::HepRandom::showEngineStatus();
    CLHEP::HepRandom::saveEngineStatus("endOfRun.rndm");
  }     

  //============================ SAVE hist ======================================
 SetName();

  printf(" SAVE hist to file: %s\n",histName.c_str());
  //TFile* f = new TFile("trd_hist.root","RECREATE");histName
  //TFile* f = new TFile(histName,"RECREATE");
  TFile* myRootfile = new TFile(RootFileName,"RECREATE");

  if (histo1) {
    printf(" Write histo1 to file %s, histo1=%p f=%p \n",histName.c_str(),histo1,myRootfile);
    histo1->Write();
  }
  if (histo2) {
    printf(" Write histo2 to file %s, histo2=%p f=%p \n",histName.c_str(),histo2,myRootfile);
    histo2->Write();
  }
  if (histo3) histo3->Write();
  if (histo4) histo4->Write();
  if (histo5) histo5->Write();
  if (histo6) histo6->Write();
  if (histo7) histo7->Write();
  if (histo8) histo8->Write();
  if (histo9) histo9->Write();
  if (histo10) histo10->Write();
  if (histo11) histo11->Write();
  if (histo12) histo12->Write();
  if (histo13) histo13->Write();
  if (histo14) histo14->Write();
  //------------------------------------------------------------
  for (int ihi=0; ihi<NHIST; ihi++) {
    printf(" Test hist%d  hist=%p f=%p \n",ihi,hist[ihi],myRootfile);

    if (d2_hist[ihi])  d2_hist[ihi]->Write();
    if (hist[ihi]) {
      hist[ihi]->Write();
      printf(" Write hist[%d] to file %s, hist=%p f=%p \n",ihi,histName.c_str(),myRootfile);
    }
  }
   //------------------------------------------------------------
 for (int in=0;in<12;in++) {    
   hmatrixOccup[in]->Write();
   hmatrixOccupCM[in]->Write();
  }


  //------------------------------------------------------------
  for (int ihi=0; ihi<NHIST; ihi++) {
    if (HLikelihood[ihi]) HLikelihood[ihi]->Write();
  }

  printf(" Close file %s, f=%p \n",histName.c_str(),myRootfile);
  myRootfile->Close();
}

///////////////////////////////////////////////////////////////////////////

void JLeicRunAction::CountEvent()
{
  TotNbofEvents += 1. ;
}

/////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddnStepsCharged(G4double ns)
{
  nStepSumCharged += ns;
  nStepSum2Charged += ns*ns;
}

////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddnStepsNeutral(G4double ns)
{
  nStepSumNeutral += ns;
  nStepSum2Neutral += ns*ns;
}

////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddEdeps(G4double Eabs)
{
  EnergySumAbs += Eabs;
  EnergySquareSumAbs += Eabs*Eabs;
}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddTrackLength(G4double tlabs)
{
  tlSumAbs += tlabs;
  tlsquareSumAbs += tlabs*tlabs ;
}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddTrRef(G4double tr,G4double ref)
{
  Transmitted += tr ;
  Reflected   += ref;
}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillNbOfSteps(G4double ns)
{
  
  const G4double eps = 1.e-10 ;
  G4double n,bin ;
  G4int ibin;

  if(histo1)
  {
    entryStep += 1. ;
 
    if(ns<Steplow)
      underStep += 1. ;
    else if(ns>=Stephigh)
      overStep  += 1. ;
    else
    {
      n = ns+eps ;
      bin = (n-Steplow)/dStep ;
      ibin= (G4int)bin ;
      distStep[ibin] += 1. ;
    }
    //histo1->accumulate(ns) ;
   histo1->Fill(ns) ;
  }
  
}
//////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillGamAngle(G4double theta)
{

 if(histo13) histo13->Fill(theta/mrad) ; 

}
//////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillGamDE(G4double de)
{

 if(histo11) histo11->Fill(de/keV) ; 

}
//////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillEn(G4double En)
{

  // #ifndef G4NOHIST

  

  G4double bin ;
  G4int ibin;

  //  if(histo2)
  {
    entryEn += 1. ;
 
    if(En < Enlow)          underEn += 1. ;
    else if( En >= Enhigh)  overEn  += 1. ;
    else
    {
      bin = (En-Enlow)/dEn ;
      ibin= (G4int)bin ;
      distEn[ibin] += 1. ;
    }
    //    histo2->accumulate(En/keV) ; // was /MeV
     if(histo2) histo2->Fill(En/keV) ; // was /MeV
  }

  // #endif

  

}

////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillTt(G4double En)
{
  
  G4double bin ;
  G4int ibin;

  if(histo5)
  {
    entryTt += 1. ;
    Ttmean += En ;
    Tt2mean += En*En ;

    if(En<Ttlow)
      underTt += 1. ;
    else if(En>=Tthigh)
      overTt  += 1. ;
    else
    {
      bin = (En-Ttlow)/dTt ;
      ibin= (G4int)bin ;
      distTt[ibin] += 1. ;
    }
    //histo5->accumulate(En/MeV) ;
  histo5->Fill(En/MeV) ;
  }
  
}

//////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillTb(G4double En)
{
  
  G4double bin ;
  G4int ibin;
  
  if(histo7)
  {
    entryTb += 1. ;
    Tbmean += En ;
    Tb2mean += En*En ;

    if(En<Tblow)
      underTb += 1. ;
    else if(En>=Tbhigh)
      overTb  += 1. ;
    else
    {
      bin = (En-Tblow)/dTb ;
      ibin= (G4int)bin ;
      distTb[ibin] += 1. ;
    }
    //histo7->accumulate(En/MeV) ;
  histo7->Fill(En/MeV) ;
  }
  
}

///////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillTsec(G4double En)
{
  
  G4double bin ;
  G4int ibin;

  if(histo8)
  {
    entryTsec += 1. ;

    if(En<Tseclow)
      underTsec += 1. ;
    else if(En>=Tsechigh)
      overTsec  += 1. ;
    else
    {
      bin = (En-Tseclow)/dTsec ;
      ibin= (G4int)bin ;
      distTsec[ibin] += 1. ;
    }
    //histo8->accumulate(En/MeV) ;
  histo8->Fill(En/MeV) ;
  }
  
}
/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillGammaInSpectrum(G4double En)
{
    histo12->Fill(En/keV) ;
}

void JLeicRunAction::FillGammaEStep(G4double En)
{

  //printf("FillGammaEStep:: fill  E=%f\n",En/keV);
    histo14->Fill(En/keV) ;
}
/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillGammaOutSpectrum(G4double En)
{
  
  G4double bin ;
  G4int ibin;

  if(histo10)
  {
    entryGamma += 1. ;
    /*
    if(En<ElowGamma)
      underGamma += 1. ;
    else if(En>=EhighGamma)
      overGamma  += 1. ;
    else
    {
      bin = std::log(En/ElowGamma)/dEGamma;
      ibin= (G4int)bin ;
      distGamma[ibin] += 1. ;
    }
    */
    //histo10->accumulate(std::log10(En/MeV)) ;
    //histo10->Fill(std::log10(En/MeV)) ;
    histo10->Fill(En/keV) ;
  }
  
}

////////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillTh(G4double Th)
{
  
  static const G4double cn=CLHEP::pi/(64800.*dTh) ;
  static const G4double cs=CLHEP::pi/
        (64800.*(std::cos(Thlow)-std::cos(Thlow+dTh)));      
  G4double bin,Thbin ,wg;
  G4int ibin;

  if(histo3)
  {
    entryTh += 1. ;

    wg = 0.;

    if(Th<Thlow)
      underTh += 1. ;
    else if(Th>=Thhigh)
      overTh  += 1. ;
    else
    {
      bin = (Th-Thlow)/dTh ;
      ibin= (G4int)bin ;
      Thbin = Thlow+ibin*dTh ;
      if(Th > 0.001*dTh)
        wg=cn/std::sin(Th) ;
      else
      {  
	G4double thdeg=Th*180./CLHEP::pi;
        G4cout << "theta < 0.001*dth (from plot excluded) theta="
               << std::setw(12) << std::setprecision(4) << thdeg << G4endl;
        wg=0. ; 
      }
      distTh[ibin] += wg  ;
    }
    //G4double thdeg=Th*180./pi;
    //G4cout << "theta="<< std::setw(12) << std::setprecision(4) << thdeg << ", wg=" << wg << G4endl;
    //histo3->accumulate(Th/deg, wg) ;
    histo3->Fill(Th/deg, wg) ;
  }
  
}

//////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillThBack(G4double Th)
{
  
  static const G4double cn=CLHEP::pi/(64800.*dThback) ;
  static const G4double cs=CLHEP::pi/
        (64800.*(std::cos(Thlowback)-std::cos(Thlowback+dThback)));      
  G4double bin,Thbin,wg ;
  G4int ibin;

  if(histo6)
  {
    entryThback += 1. ;

    if(Th<Thlowback)
      underThback += 1. ;
    else if(Th>=Thhighback)
      overThback  += 1. ;
    else
    {
      bin = (Th-Thlowback)/dThback ;
      ibin= (G4int)bin ;
      Thbin = Thlowback+ibin*dThback ;
      if(Th > 0.001*dThback)
        wg=cn/std::sin(Th) ;
      else
      {  
        G4double thdeg=Th*180./CLHEP::pi;
        G4cout << "theta < 0.001*dth (from plot excluded) theta="
               << std::setw(12) << std::setprecision(4) << thdeg << G4endl;
        wg=0. ; 
      }
      distThback[ibin] += wg  ;
    }
    //histo6->accumulate(Th/deg, wg) ;
  histo6->Fill(Th/deg, wg) ;
  }
  
}

//////////////////////////////////////////////////////////////////////////

void JLeicRunAction::FillR(G4double R )
{
  
  G4double bin ;
  G4int ibin;

  if(histo4)
  {
    entryR  += 1. ;
    Rmean += R ;
    R2mean += R*R ;

    if(R <Rlow)
      underR  += 1. ;
    else if(R >=Rhigh)
      overR   += 1. ;
    else
    {
      bin = (R -Rlow)/dR  ;
      ibin= (G4int)bin ;
      distR[ibin] += 1. ;
    }
    //histo4->accumulate(R/mm) ;
  histo4->Fill(R/mm) ;
  }
  
}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::Fillvertexz(G4double z )
{
  
  G4double bin ;
  G4int ibin;
  
  if(histo9)
  {
    entryvertexz  += 1. ;

    if(z <zlow)
      undervertexz  += 1. ;
    else if(z >=zhigh)
      oververtexz   += 1. ;
    else
    {
      bin = (z -zlow)/dz  ;
      ibin = (G4int)bin ;
      distvertexz[ibin] += 1. ;
    }
    //histo9->accumulate(z/mm) ;
  histo9->Fill(z/mm) ;
  }
  
}

//////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::SethistName(G4String name)
{
  histName = name ;
  G4cout << " hist file = " << histName << G4endl;
}

void JLeicRunAction::SetnbinStep(G4int nbin)
{
  nbinStep = nbin ;
  if(nbinStep>0)
  G4cout << " Nb of bins in #step plot = " << nbinStep << G4endl ;
}

void JLeicRunAction::SetSteplow(G4double low)
{
  Steplow = low ;
  if(nbinStep>0)
  G4cout << " low  in the #step plot = " << Steplow << G4endl ;
}
void JLeicRunAction::SetStephigh(G4double high)
{
  Stephigh = high ;
  if(nbinStep>0)
  G4cout << " high in the #step plot = " << Stephigh << G4endl ;
}

////////////////////////////////////////////////////////////////////////

void JLeicRunAction::SetnbinEn(G4int nbin)
{
  nbinEn = nbin ;

  if(nbinEn > 0) G4cout << " Nb of bins in Edep plot = " << nbinEn << G4endl ;
}

void JLeicRunAction::SetEnlow(G4double Elow)
{
  Enlow = Elow ;
  if(nbinEn>0) G4cout << " Elow  in the  Edep plot = " << Enlow << G4endl ;
}

void JLeicRunAction::SetEnhigh(G4double Ehigh)
{
  Enhigh = Ehigh ;

  if(nbinEn>0) G4cout << " Ehigh in the  Edep plot = " << Enhigh << G4endl ;
}

/////////////////////////////////////////////////////////////////////////

void JLeicRunAction::SetnbinGamma(G4int nbin)
{
  nbinGamma = nbin ;
  if(nbinGamma>0)
  G4cout << " Nb of bins in gamma spectrum plot = " << nbinGamma << G4endl ;
}

void JLeicRunAction::SetElowGamma(G4double Elow)
{
  ElowGamma = Elow ;
  if(nbinGamma>0)
  G4cout << " Elow  in the gamma spectrum plot = " << ElowGamma << G4endl ;
}

void JLeicRunAction::SetEhighGamma(G4double Ehigh)
{
  EhighGamma = Ehigh ;
  if(nbinGamma>0)
  G4cout << " Ehigh in the gamma spectrum plot = " << EhighGamma << G4endl ;
}

void JLeicRunAction::SetnbinTt(G4int nbin)
{
  nbinTt = nbin ;
  if(nbinTt>0)
  G4cout << " Nb of bins in Etransmisssion plot = " << nbinTt << G4endl ;
}

void JLeicRunAction::SetTtlow(G4double Elow)
{
  Ttlow = Elow ;
  if(nbinTt>0)
  G4cout << " Elow  in the  Etransmission plot = " << Ttlow << G4endl ;
}

void JLeicRunAction::SetTthigh(G4double Ehigh)
{
  Tthigh = Ehigh ;
  if(nbinTt>0)
  G4cout << " Ehigh in the  Etransmission plot = " << Tthigh << G4endl ;
}

void JLeicRunAction::SetnbinTb(G4int nbin)
{
  nbinTb = nbin ;
  if(nbinTb>0)
  G4cout << " Nb of bins in Ebackscattered plot = " << nbinTb << G4endl ;
}

void JLeicRunAction::SetTblow(G4double Elow)
{
  Tblow = Elow ;
  if(nbinTb>0)
  G4cout << " Elow  in the  Ebackscattered plot = " << Tblow << G4endl ;
}

void JLeicRunAction::SetTbhigh(G4double Ehigh)
{
  Tbhigh = Ehigh ;
  if(nbinTb>0)
  G4cout << " Ehigh in the  Ebackscattered plot = " << Tbhigh << G4endl ;
}

void JLeicRunAction::SetnbinTsec(G4int nbin)
{
  nbinTsec = nbin ;
  if(nbinTsec>0)
  G4cout << " Nb of bins in Tsecondary  plot = " << nbinTsec << G4endl ;
}

void JLeicRunAction::SetTseclow(G4double Elow)
{
  Tseclow = Elow ;
  if(nbinTsec>0)
  G4cout << " Elow  in the  Tsecondary plot = " << Tseclow << G4endl ;
}

void JLeicRunAction::SetTsechigh(G4double Ehigh)
{
  Tsechigh = Ehigh ;
  if(nbinTsec>0)
  G4cout << " Ehigh in the  Tsecondary plot = " << Tsechigh << G4endl ;
}
 
void JLeicRunAction::SetnbinR(G4int nbin)
{
  nbinR  = nbin ;
  if(nbinR>0)
  G4cout << " Nb of bins in R plot = " << nbinR << G4endl ;
}

void JLeicRunAction::SetRlow(G4double rlow)
{
  Rlow = rlow ;
  if(nbinR>0)
  G4cout << " Rlow  in the  R plot = " << Rlow << G4endl ;
}

void JLeicRunAction::SetRhigh(G4double rhigh)
{
  Rhigh = rhigh ;
  if(nbinR>0)
  G4cout << " Rhigh in the R plot = " << Rhigh << G4endl ;
}

void JLeicRunAction::Setnbinzvertex(G4int nbin)
{
  nbinvertexz  = nbin ;
  if(nbinvertexz>0)
  G4cout << " Nb of bins in Z plot = " << nbinvertexz << G4endl ;
}

void JLeicRunAction::Setzlow(G4double z)
{
  zlow = z ;
  if(nbinvertexz>0)
  G4cout << " zlow  in the  Z plot = " << zlow << G4endl ;
}

void JLeicRunAction::Setzhigh(G4double z)
{
  zhigh = z ;
  if(nbinvertexz>0)
  G4cout << " zhigh in the Z plot = " << zhigh << G4endl ;
}

void JLeicRunAction::SetnbinTh(G4int nbin)
{
  nbinTh = nbin ;
  if(nbinTh>0)
  G4cout << " Nb of bins in Theta plot = " << nbinTh << G4endl ;
}

void JLeicRunAction::SetThlow(G4double Tlow)
{
  Thlow = Tlow ;
  if(nbinTh>0)
  G4cout << " Tlow  in the  Theta plot = " << Thlow << G4endl ;
}

void JLeicRunAction::SetThhigh(G4double Thigh)
{
  Thhigh = Thigh ;
  if(nbinTh>0)
  G4cout << " Thigh in the Theta plot = " << Thhigh << G4endl ;
}

void JLeicRunAction::SetnbinThBack(G4int nbin)
{
  nbinThback = nbin ;
  if(nbinThback>0)
  G4cout << " Nb of bins in Theta plot = " << nbinThback << G4endl ;
}

void JLeicRunAction::SetThlowBack(G4double Tlow)
{
  Thlowback = Tlow ;
  if(nbinThback>0)
  G4cout << " Tlow  in the  Theta plot = " << Thlowback << G4endl ;
}

void JLeicRunAction::SetThhighBack(G4double Thigh)
{
  Thhighback = Thigh ;
  if(nbinThback>0)
  G4cout << " Thigh in the Theta plot = " << Thhighback << G4endl ;
}

void JLeicRunAction::CountParticles(G4double nch,G4double nne)
{
  SumCharged += nch ;
  SumNeutral += nne ;
  Sum2Charged += nch*nch ;
  Sum2Neutral += nne*nne ;
}

void JLeicRunAction::AddEP(G4double nele,G4double npos) 
{
  Selectron += nele;
  Spositron += npos;
}

//
//
////////////////////////////////////////////////////////////////////////
