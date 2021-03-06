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

#include <string>

#include "JLeicRunAction.hh"
#include "JLeicHistoMConfig.hh"
#include "JLeicHistogramManager.hh"
#include "JLeicDetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

// Geant4 includes
#include <G4Run.hh>
#include <G4UImanager.hh>
#include <G4VVisManager.hh>
#include <G4ios.hh>
#include <Randomize.hh>
#include <G4SystemOfUnits.hh>

#include "rootlib.h"


#include <TFile.h>

JLeicRunAction::JLeicRunAction(g4e::RootFlatIO* jLeicRootOutput, JLeicHistogramManager *histos) :
    mRootEventsOut(jLeicRootOutput),
    histName("histfile"),
    nbinStep(0), nbinEn(0), nbinTt(0), nbinTb(0), nbinTsec(0), nbinTh(0),
    nbinThback(0), nbinR(0), nbinGamma(0), nbinvertexz(0), fHistos(histos)
{
    runMessenger = new JLeicHistoMConfig();
    saveRndm = 1;
}


JLeicRunAction::~JLeicRunAction()
{
    delete runMessenger;
}



void JLeicRunAction::BeginOfRunAction(const G4Run *aRun)
{

    printf("RunAction:: Enter\n");
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4UImanager *UI = G4UImanager::GetUIpointer();

    G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();

    if (pVVisManager) UI->ApplyCommand("/vis/scene/notifyHandlers");


    EnergySumAbs = 0.;
    EnergySquareSumAbs = 0.;
    tlSumAbs = 0.;
    tlsquareSumAbs = 0.;
    nStepSumCharged = 0.;
    nStepSum2Charged = 0.;
    nStepSumNeutral = 0.;
    nStepSum2Neutral = 0.;
    TotNbofEvents = 0.;
    SumCharged = 0.;
    SumNeutral = 0.;
    Sum2Charged = 0.;
    Sum2Neutral = 0.;
    Selectron = 0.;
    Spositron = 0.;

    Transmitted = 0.;
    Reflected = 0.;

//  plot definitions 
    printf("RunAction:: 2\n");

    if (nbinStep > 0) {
        dStep = (Stephigh - Steplow) / nbinStep;
        entryStep = 0.;
        underStep = 0.;
        overStep = 0.;
        for (G4int ist = 0; ist < nbinStep; ist++) {
            distStep[ist] = 0.;
        }
    }
    if (nbinEn > 0) {
        dEn = (Enhigh - Enlow) / nbinEn;
        entryEn = 0.;
        underEn = 0.;
        overEn = 0.;

        for (G4int ien = 0; ien < nbinEn; ien++) distEn[ien] = 0.;
    }
    if (nbinTt > 0) {
        dTt = (Tthigh - Ttlow) / nbinTt;
        entryTt = 0.;
        underTt = 0.;
        overTt = 0.;

        printf("RunAction:: 3\n");

        for (G4int itt = 0; itt < nbinTt; itt++) distTt[itt] = 0.;

        Ttmean = 0.;
        Tt2mean = 0.;
    }
    if (nbinTb > 0) {
        dTb = (Tbhigh - Tblow) / nbinTb;
        entryTb = 0.;
        underTb = 0.;
        overTb = 0.;
        for (G4int itt = 0; itt < nbinTb; itt++) {
            distTb[itt] = 0.;
        }
        Tbmean = 0.;
        Tb2mean = 0.;
    }
    if (nbinTsec > 0) {
        dTsec = (Tsechigh - Tseclow) / nbinTsec;
        entryTsec = 0.;
        underTsec = 0.;
        overTsec = 0.;
        for (G4int its = 0; its < nbinTsec; its++) {
            distTsec[its] = 0.;
        }
    }
    if (nbinTh > 0) {
        dTh = (Thhigh - Thlow) / nbinTh;
        entryTh = 0.;
        underTh = 0.;
        overTh = 0.;
        for (G4int ith = 0; ith < nbinTh; ith++) {
            distTh[ith] = 0.;
        }
    }
    printf("RunAction:: 4\n");

    if (nbinThback > 0) {
        dThback = (Thhighback - Thlowback) / nbinThback;
        entryThback = 0.;
        underThback = 0.;
        overThback = 0.;
        for (G4int ithback = 0; ithback < nbinThback; ithback++) {
            distThback[ithback] = 0.;
        }
    }


    if (nbinR > 0) {
        dR = (Rhigh - Rlow) / nbinR;
        entryR = 0.;
        underR = 0.;
        overR = 0.;
        for (G4int ir = 0; ir < nbinR; ir++) {
            distR[ir] = 0.;
        }
        Rmean = 0.;
        R2mean = 0.;
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


    if (nbinvertexz > 0) {
        dz = (zhigh - zlow) / nbinvertexz;
        entryvertexz = 0.;
        undervertexz = 0.;
        oververtexz = 0.;
        for (G4int iz = 0; iz < nbinvertexz; iz++) {
            distvertexz[iz] = 0.;
        }
    }
}


/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::EndOfRunAction(const G4Run *)
{
    G4double sAbs, sigAbs, sigstep, sigcharged, signeutral;


    tlSumAbs /= TotNbofEvents;
    sAbs = tlsquareSumAbs / TotNbofEvents - tlSumAbs * tlSumAbs;
    if (sAbs > 0.)
        sAbs = std::sqrt(sAbs / TotNbofEvents);
    else
        sAbs = 0.;

    EnergySumAbs /= TotNbofEvents;
    sigAbs = EnergySquareSumAbs / TotNbofEvents - EnergySumAbs * EnergySumAbs;
    if (sigAbs > 0.)
        sigAbs = std::sqrt(sigAbs / TotNbofEvents);
    else
        sigAbs = 0.;

    nStepSumCharged /= TotNbofEvents;
    sigstep = nStepSum2Charged / TotNbofEvents - nStepSumCharged * nStepSumCharged;
    if (sigstep > 0.)
        sigstep = std::sqrt(sigstep / TotNbofEvents);
    else
        sigstep = 0.;
    G4double sigch = sigstep;

    nStepSumNeutral /= TotNbofEvents;
    sigstep = nStepSum2Neutral / TotNbofEvents - nStepSumNeutral * nStepSumNeutral;
    if (sigstep > 0.)
        sigstep = std::sqrt(sigstep / TotNbofEvents);
    else
        sigstep = 0.;
    G4double signe = sigstep;

    SumCharged /= TotNbofEvents;
    sigcharged = Sum2Charged / TotNbofEvents - SumCharged * SumCharged;
    if (sigcharged > 0.)
        sigcharged = std::sqrt(sigcharged / TotNbofEvents);
    else
        sigcharged = 0.;

    SumNeutral /= TotNbofEvents;
    signeutral = Sum2Neutral / TotNbofEvents - SumNeutral * SumNeutral;
    if (signeutral > 0.)
        signeutral = std::sqrt(signeutral / TotNbofEvents);
    else
        signeutral = 0.;

    Selectron /= TotNbofEvents;
    Spositron /= TotNbofEvents;

    Transmitted /= TotNbofEvents;
    Reflected /= TotNbofEvents;
    G4cout << " ================== run summary =====================" << G4endl;
    G4int prec = G4cout.precision(6);
    G4cout << " end of Run TotNbofEvents = " << TotNbofEvents << G4endl;
    G4cout << "    mean charged track length   in absorber=" << tlSumAbs / mm << " +- " << sAbs / mm << "  mm  " << G4endl;
    G4cout << G4endl;
    G4cout << "            mean energy deposit in absorber=" << EnergySumAbs / MeV << " +- " << sigAbs / MeV << "  MeV " << G4endl;
    G4cout << G4endl;
    G4cout << " mean number of steps in absorber (charged) =" << nStepSumCharged << " +- " << sigch << "      " << G4endl;
    G4cout << " mean number of steps in absorber (neutral) =" << nStepSumNeutral << " +- " << signe << "      " << G4endl;
    G4cout << G4endl;
    G4cout << "   mean number of charged secondaries = " << SumCharged << " +- " << sigcharged << G4endl;
    G4cout << G4endl;
    G4cout << "   mean number of neutral secondaries = " << SumNeutral << " +- " << signeutral << G4endl;
    G4cout << G4endl;

    G4cout << "   mean number of e-s =" << Selectron << "  and e+s =" << Spositron << G4endl;
    G4cout << G4endl;

    G4cout << "(number) transmission coeff=" << Transmitted << "  reflection coeff=" << Reflected << G4endl;
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

    if (G4VVisManager::GetConcreteInstance()) {
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    }

    auto myRootfile = new TFile("histos.root", "RECREATE");
    myRootfile->cd();
    // fHistos->Write();
    myRootfile->Close();


    //if (mHitsFile) {
//        mHitsFile->cd();

        mRootEventsOut->Write();
  //      mHitsFile->Close();
//    }


}

///////////////////////////////////////////////////////////////////////////

void JLeicRunAction::CountEvent()
{
    TotNbofEvents += 1.;
}

/////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddnStepsCharged(G4double steps)
{
    nStepSumCharged += steps;
    nStepSum2Charged += steps * steps;
}

////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddnStepsNeutral(G4double steps)
{
    nStepSumNeutral += steps;
    nStepSum2Neutral += steps * steps;
}

////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddEdeps(G4double Eabs)
{
    EnergySumAbs += Eabs;
    EnergySquareSumAbs += Eabs * Eabs;
}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddTrackLength(G4double tlabs)
{
    tlSumAbs += tlabs;
    tlsquareSumAbs += tlabs * tlabs;
}

/////////////////////////////////////////////////////////////////////////////

void JLeicRunAction::AddTrRef(G4double tr, G4double ref)
{
    Transmitted += tr;
    Reflected += ref;
}
//
///////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillNbOfSteps(G4double ns)
//{
//
//    const G4double eps = 1.e-10;
//    G4double n, bin;
//    G4int ibin;
//
//    if (histo1) {
//        entryStep += 1.;
//
//        if (ns < Steplow)
//            underStep += 1.;
//        else if (ns >= Stephigh)
//            overStep += 1.;
//        else {
//            n = ns + eps;
//            bin = (n - Steplow) / dStep;
//            ibin = (G4int) bin;
//            distStep[ibin] += 1.;
//        }
//        //histo1->accumulate(ns) ;
//        histo1->Fill(ns);
//    }
//
//}
////////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillGamAngle(G4double theta)
//{
//
//    if (histo13) histo13->Fill(theta / mrad);
//
//}
////////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillGamDE(G4double de)
//{
//
//    if (histo11) histo11->Fill(de / keV);
//
//}
////////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillEn(G4double En)
//{
//
//    // #ifndef G4NOHIST
//
//
//
//    G4double bin;
//    G4int ibin;
//
//    //  if(histo2)
//    {
//        entryEn += 1.;
//
//        if (En < Enlow) underEn += 1.;
//        else if (En >= Enhigh) overEn += 1.;
//        else {
//            bin = (En - Enlow) / dEn;
//            ibin = (G4int) bin;
//            distEn[ibin] += 1.;
//        }
//        //    histo2->accumulate(En/keV) ; // was /MeV
//        if (histo2) histo2->Fill(En / keV); // was /MeV
//    }
//
//    // #endif
//
//
//
//}
//
//////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillTt(G4double En)
//{
//
//    G4double bin;
//    G4int ibin;
//
//    if (histo5) {
//        entryTt += 1.;
//        Ttmean += En;
//        Tt2mean += En * En;
//
//        if (En < Ttlow)
//            underTt += 1.;
//        else if (En >= Tthigh)
//            overTt += 1.;
//        else {
//            bin = (En - Ttlow) / dTt;
//            ibin = (G4int) bin;
//            distTt[ibin] += 1.;
//        }
//        //histo5->accumulate(En/MeV) ;
//        histo5->Fill(En / MeV);
//    }
//
//}
//
////////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillTb(G4double En)
//{
//
//    G4double bin;
//    G4int ibin;
//
//    if (histo7) {
//        entryTb += 1.;
//        Tbmean += En;
//        Tb2mean += En * En;
//
//        if (En < Tblow)
//            underTb += 1.;
//        else if (En >= Tbhigh)
//            overTb += 1.;
//        else {
//            bin = (En - Tblow) / dTb;
//            ibin = (G4int) bin;
//            distTb[ibin] += 1.;
//        }
//        //histo7->accumulate(En/MeV) ;
//        histo7->Fill(En / MeV);
//    }
//
//}
//
/////////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillTsec(G4double En)
//{
//
//    G4double bin;
//    G4int ibin;
//
//    if (histo8) {
//        entryTsec += 1.;
//
//        if (En < Tseclow)
//            underTsec += 1.;
//        else if (En >= Tsechigh)
//            overTsec += 1.;
//        else {
//            bin = (En - Tseclow) / dTsec;
//            ibin = (G4int) bin;
//            distTsec[ibin] += 1.;
//        }
//        //histo8->accumulate(En/MeV) ;
//        histo8->Fill(En / MeV);
//    }
//
//}
///////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillGammaInSpectrum(G4double En)
//{
//    histo12->Fill(En / keV);
//}
//
//void JLeicRunAction::FillGammaEStep(G4double En)
//{
//
//    //printf("FillGammaEStep:: fill  E=%f\n",En/keV);
//    histo14->Fill(En / keV);
//}
///////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillGammaOutSpectrum(G4double En)
//{
//
//    G4double bin;
//    G4int ibin;
//
//    if (histo10) {
//        entryGamma += 1.;
//        /*
//        if(En<ElowGamma)
//          underGamma += 1. ;
//        else if(En>=EhighGamma)
//          overGamma  += 1. ;
//        else
//        {
//          bin = std::log(En/ElowGamma)/dEGamma;
//          ibin= (G4int)bin ;
//          distGamma[ibin] += 1. ;
//        }
//        */
//        //histo10->accumulate(std::log10(En/MeV)) ;
//        //histo10->Fill(std::log10(En/MeV)) ;
//        histo10->Fill(En / keV);
//    }
//
//}
//
//////////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillTh(G4double Th)
//{
//
//    static const G4double cn = CLHEP::pi / (64800. * dTh);
//    static const G4double cs = CLHEP::pi / (64800. * (std::cos(Thlow) - std::cos(Thlow + dTh)));
//    G4double bin, Thbin, wg;
//    G4int ibin;
//
//    if (histo3) {
//        entryTh += 1.;
//
//        wg = 0.;
//
//        if (Th < Thlow)
//            underTh += 1.;
//        else if (Th >= Thhigh)
//            overTh += 1.;
//        else {
//            bin = (Th - Thlow) / dTh;
//            ibin = (G4int) bin;
//            Thbin = Thlow + ibin * dTh;
//            if (Th > 0.001 * dTh)
//                wg = cn / std::sin(Th);
//            else {
//                G4double thdeg = Th * 180. / CLHEP::pi;
//                G4cout << "theta < 0.001*dth (from plot excluded) theta=" << std::setw(12) << std::setprecision(4) << thdeg << G4endl;
//                wg = 0.;
//            }
//            distTh[ibin] += wg;
//        }
//        //G4double thdeg=Th*180./pi;
//        //G4cout << "theta="<< std::setw(12) << std::setprecision(4) << thdeg << ", wg=" << wg << G4endl;
//        //histo3->accumulate(Th/deg, wg) ;
//        histo3->Fill(Th / deg, wg);
//    }
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillThBack(G4double Th)
//{
//
//    static const G4double cn = CLHEP::pi / (64800. * dThback);
//    static const G4double cs = CLHEP::pi / (64800. * (std::cos(Thlowback) - std::cos(Thlowback + dThback)));
//    G4double bin, Thbin, wg;
//    G4int ibin;
//
//    if (histo6) {
//        entryThback += 1.;
//
//        if (Th < Thlowback)
//            underThback += 1.;
//        else if (Th >= Thhighback)
//            overThback += 1.;
//        else {
//            bin = (Th - Thlowback) / dThback;
//            ibin = (G4int) bin;
//            Thbin = Thlowback + ibin * dThback;
//            if (Th > 0.001 * dThback)
//                wg = cn / std::sin(Th);
//            else {
//                G4double thdeg = Th * 180. / CLHEP::pi;
//                G4cout << "theta < 0.001*dth (from plot excluded) theta=" << std::setw(12) << std::setprecision(4) << thdeg << G4endl;
//                wg = 0.;
//            }
//            distThback[ibin] += wg;
//        }
//        //histo6->accumulate(Th/deg, wg) ;
//        histo6->Fill(Th / deg, wg);
//    }
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::FillR(G4double R)
//{
//
//    G4double bin;
//    G4int ibin;
//
//    if (histo4) {
//        entryR += 1.;
//        Rmean += R;
//        R2mean += R * R;
//
//        if (R < Rlow)
//            underR += 1.;
//        else if (R >= Rhigh)
//            overR += 1.;
//        else {
//            bin = (R - Rlow) / dR;
//            ibin = (G4int) bin;
//            distR[ibin] += 1.;
//        }
//        //histo4->accumulate(R/mm) ;
//        histo4->Fill(R / mm);
//    }
//
//}
//
///////////////////////////////////////////////////////////////////////////////
//
//void JLeicRunAction::Fillvertexz(G4double z)
//{
//
//    G4double bin;
//    G4int ibin;
//
//    if (histo9) {
//        entryvertexz += 1.;
//
//        if (z < zlow)
//            undervertexz += 1.;
//        else if (z >= zhigh)
//            oververtexz += 1.;
//        else {
//            bin = (z - zlow) / dz;
//            ibin = (G4int) bin;
//            distvertexz[ibin] += 1.;
//        }
//        //histo9->accumulate(z/mm) ;
//        histo9->Fill(z / mm);
//    }
//
//}


void JLeicRunAction::CountParticles(G4double nch, G4double nne)
{
    SumCharged += nch;
    SumNeutral += nne;
    Sum2Charged += nch * nch;
    Sum2Neutral += nne * nne;
}

void JLeicRunAction::AddEP(G4double nele, G4double npos)
{
    Selectron += nele;
    Spositron += npos;
}

//
//
////////////////////////////////////////////////////////////////////////
