//
// Created by romanov on 11/24/19.
//

#ifndef G4E_JLEICHISTOGRAMMING_HH
#define G4E_JLEICHISTOGRAMMING_HH

#include <TH1D.h>
#include <TH2D.h>

#include <globals.hh>
#include <G4SIunits.hh>

class JLeicHistogramming {
public:

    static TH1D *histo1, *histo2, *histo3, *histo4, *histo5, *histo6, *histo7, *histo8, *histo9, *histo10, *histo11, *histo12, *histo13, *histo14;
//static TH2D* d2_pos_ffq1;
    static const int NHIST = 50;
    TH1D *hist[NHIST];

    TH1D *HLikelihood[NHIST];
    char Hname[256];
     TH2D *d2_hist[NHIST];


     int NumRow = 10;
     int NumCol = 50;
     TH2F *hmatrixOccup[120];
     TH2F *hmatrixOccupCM[120];


    void FillHLikelihood(int ihist, G4double val)
    {

        //printf("FillHLikelihood(%d, %f)  ptr=%p\n",ihist,val,HLikelihood[ihist]);
        if (ihist < NHIST && HLikelihood[ihist]) HLikelihood[ihist]->Fill(val);

    }

    void FillHist(int ihist, G4double val)
    {

        if (hist[ihist]) hist[ihist]->Fill(val);

    }

    void FillHist(int ihist, G4double val, G4double w)
    {

        if (hist[ihist]) hist[ihist]->Fill(val, w);

    }

    void FillHist(int ihist, int bin, G4double w)
    {

        if (hist[ihist]) hist[ihist]->Fill(bin, w);

    }

    void ResetHist(int ihist)
    {

        if (hist[ihist]) hist[ihist]->Reset();

    }

//---  2D ---
    void FillHist2d(int ihist, G4double valx, G4double valy, G4double w)
    {

        if (d2_hist[ihist]) d2_hist[ihist]->Fill(valx, valy, w);

    }

    G4double GetHist2d(int ihist, G4int ix, G4int iy)
    {

        if (d2_hist[ihist]) return d2_hist[ihist]->GetBinContent(ix, iy);

    }

    void ResetHist2d(int ihist)
    {

        if (d2_hist[ihist]) d2_hist[ihist]->Reset();

    }

    void FillHistmatrixOccup(int ihist, G4double valx, G4double valy, G4double w)
    {

        // printf("ihist= %d\n",ihist);
        //  if( hmatrixOccup[ihist])  hmatrixOccup[ihist]->Fill(valx,valy,w) ;
        if (hmatrixOccup[ihist]) hmatrixOccup[ihist]->Fill(valx, valy, 1.);

    }

    void FillHistmatrixOccupCM(int ihist, G4double valx, G4double valy, G4double w)
    {

        // printf("ihist= %d\n",ihist);
        //  if( hmatrixOccup[ihist])  hmatrixOccup[ihist]->Fill(valx,valy,w) ;
        if (hmatrixOccupCM[ihist]) hmatrixOccupCM[ihist]->Fill(valx, valy, 1.);

    }


    JLeicHistogramming()
    {
        char myname[120];


        printf("------------------- > Booking histograms ...  \n");

        // init hbook

        //  hbookManager = new HBookFile(histName, 68);
        //  assert (hbookManager != 0);

        // book histograms

        if (nbinStep > 0) {
            printf("book histo1 \n");
            histo1 = new TH1D("histo1", "number of steps/event", nbinStep, Steplow, Stephigh);
            //histo1 = hbookManager->histogram("number of steps/event",nbinStep,Steplow,Stephigh) ;
            //assert (histo1 != 0);
        }
        if (nbinEn > 0) {
            printf("book histo2 \n");
            histo2 = new TH1D("histo2", "Energy Loss (keV)", nbinEn, Enlow / keV, Enhigh / keV);
            //histo2 = hbookManager->histogram("Energy Loss (keV)",nbinEn,Enlow/keV,Enhigh/keV) ;
            //assert (histo2 != 0);
        }
        if (nbinTh > 0) {
            histo3 = new TH1D("histo3", "angle distribution at exit(deg)", nbinTh, Thlow / deg, Thhigh / deg);
            //histo3 = hbookManager->histogram("angle distribution at exit(deg)",nbinTh,Thlow/deg,Thhigh/deg) ;
            //assert (histo3 != 0);
        }
        if (nbinR > 0) {
            histo4 = new TH1D("histo4", "lateral distribution at exit(mm)", nbinR, Rlow, Rhigh);
            //histo4 = hbookManager->histogram("lateral distribution at exit(mm)",nbinR ,Rlow,Rhigh)  ;
            //assert (histo4 != 0);
        }
        if (nbinTt > 0) {
            histo5 = new TH1D("histo5", "kinetic energy of the primary at exit(MeV)", nbinTt, Ttlow, Tthigh);
            //histo5 = hbookManager->histogram("kinetic energy of the primary at exit(MeV)",nbinTt,Ttlow,Tthigh)  ;
            //assert (histo5 != 0);
        }
        if (nbinThback > 0) {
            histo6 = new TH1D("histo6", "angle distribution of backscattered primaries(deg)", nbinThback, Thlowback / deg, Thhighback / deg);
            //histo6 = hbookManager->histogram("angle distribution of backscattered primaries(deg)",nbinThback,Thlowback/deg,Thhighback/deg) ;
            //assert (histo6 != 0);
        }
        if (nbinTb > 0) {
            histo7 = new TH1D("histo7", "kinetic energy of the backscattered primaries (MeV)", nbinTb, Tblow, Tbhigh);
            //histo7 = hbookManager->histogram("kinetic energy of the backscattered primaries (MeV)",nbinTb,Tblow,Tbhigh)  ;
            //assert (histo7 != 0);
        }
        if (nbinTsec > 0) {
            histo8 = new TH1D("histo8", "kinetic energy of the charged secondaries (keV)", nbinTsec, Tseclow, Tsechigh);
            //histo8 = hbookManager->histogram("kinetic energy of the charged secondaries (MeV)",nbinTsec,Tseclow,Tsechigh)  ;
            //assert (histo8 != 0);
        }
        if (nbinvertexz > 0) {
            histo9 = new TH1D("histo9", "z of secondary charged vertices(mm)", nbinvertexz, zlow, zhigh);
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
        printf("book  nbin=%d Elow=%f Ehigh=%f\n", nbinGamma, ElowGamma, EhighGamma);

        int nb = 500;
        float emin = 0, emax = 50.;
        histo10 = new TH1D("histo10", "kinetic energy of gammas escaping the absorber (keV)", nb, emin, emax);
        histo11 = new TH1D("histo11", "dE of gammas in the absorber (keV) ", nb, emin, emax);   //-- keV
        histo12 = new TH1D("histo12", "kinetic energy of gammas entering the absorber (keV) ", nb, emin, emax);   //-- keV
        histo14 = new TH1D("histo14", "kinetic energy of gammas in step (keV) ", nb, emin, emax);   //-- keV


        histo13 = new TH1D("histo13", "gamma angle (mrad) ", 500, 0., 12.);   //-- mrad

        hist[1] = new TH1D("hist1", "N Photon Absorption vs Z (mm)", 100, 0., 50.);
        hist[2] = new TH1D("hist2", "E Photon Absorption vs Z (mm)", 100, 0., 50.);
        hist[3] = new TH1D("hist3", "Step length charged, um ", 100, 0., 1000.);
        hist[4] = new TH1D("hist4", "dE/dx sec vs Z  mm", 100, 0., 50.);
        hist[5] = new TH1D("hist5", "dE/dx prim vs Z mm", 100, 0., 50.);
        hist[6] = new TH1D("hist6", "dE/dx charged vs Z mm", 100, 0., 50.);
        hist[7] = new TH1D("hist7", "dE/dx vs Z mm", 100, 0., 50.);

        hist[10] = new TH1D("hist10", "dE/dx total (keV) ", 200, 0., 200);   //-- keV
        nb = 100;
        emin = 0;
        emax = 20.;
        hist[11] = new TH1D("hist11", "dE/dx slice1 (keV) ", nb, emin, emax);   //-- keV
        hist[12] = new TH1D("hist12", "dE/dx slice2 (keV) ", nb, emin, emax);   //-- keV
        hist[13] = new TH1D("hist13", "dE/dx slice3 (keV) ", nb, emin, emax);   //-- keV
        for (int ii = 10; ii <= 13; ii++) hist[ii]->GetXaxis()->SetTitle("Energy , keV");

        hist[20] = new TH1D("hist20", " N TR photons bin:1-100", 50, -0.5, 49.5);
        hist[20]->GetXaxis()->SetTitle("Number TR");
        hist[21] = new TH1D("hist21", " N TR photons bin:1-5 ", 50, -0.5, 49.5);
        hist[21]->GetXaxis()->SetTitle("Number TR");
        hist[22] = new TH1D("hist22", " N TR photons bin:1-10", 50, -0.5, 49.5);
        hist[22]->GetXaxis()->SetTitle("Number TR");
        hist[23] = new TH1D("hist23", " N TR photons bin:1-15", 50, -0.5, 49.5);
        hist[23]->GetXaxis()->SetTitle("Number TR");

        hist[25] = new TH1D("hist25", "dE/dx charged  PIXEL", 1000, 0., 1000.);
        hist[26] = new TH1D("hist26", "dE/dx charged STRIP ", 1000, 0., 1000.);
        hist[27] = new TH1D("hist27", "Energy deposit gamma", 1000, 0., 1000.);


        d2_hist[1] = new TH2D("Hist2d1", "dE/dx spectrum in slices ", 50, emin, emax, 40, -0.5, 39.5);
        d2_hist[2] = new TH2D("Hist2d2", "dE/dx  vs energy PIXEL ", 1000, 0., 1000., 1000, 0., 1000.);
        d2_hist[3] = new TH2D("Hist2d3", "dE/dx  vs energy STRIP", 1000, 0., 1000., 1000, 0., 5000.);

        // ---- position at the  entrance of dipole -----
        int dminY = 40, nbinY = dminY * 2, dmaxY;
        int nbinX = 0, dminX, dmaxX, dmX = 0;

        dmX = 500;
        nbinX = dmX * 2;
        dminX = -254 - dmX;
        dmaxX = -254 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[4] = new TH2D("d2_hist4_iBDS1a_in", " d2_pos_iBDS1a_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        dmX = 500;
        nbinX = dmX * 2;
        dminX = -254 - dmX;
        dmaxX = -254 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[5] = new TH2D("d2_hist5_iBDS1b_in", " d2_pos_iBDS1b_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        dmX = 400;
        nbinX = dmX * 2;
        dminX = -1163 - dmX;
        dmaxX = -1163 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[6] = new TH2D("d2_hist6_iBDS2_in", " d2_pos_iBDS2_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        dmX = 50;
        nbinX = dmX * 2;
        dminX = -1358 - dmX;
        dmaxX = -1358 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[7] = new TH2D("d2_hist7_iBDS3_in", " d2_pos_iBDS3_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);

        // ---- position at the entrance of quadr -----
        dmX = 100;
        nbinX = dmX * 2;
        dminX = -360 - dmX;
        dmaxX = -360 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[8] = new TH2D("d2_hist8_iQDS1a_in", " d2_pos_iQDS1a_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[9] = new TH2D("d2_hist9_iQDS1S_in", " d2_pos_iQDS1S_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        dmX = 140;
        nbinX = dmX * 2;
        dminX = -532 - dmX;
        dmaxX = -532 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[10] = new TH2D("d2_hist10_iQDS1b_in", " d2_pos_iQDS1b_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[11] = new TH2D("d2_hist11_iQDS2S_in", " d2_pos_iQDS2S_in ", nbinX, -670., -610., nbinY, dminY, dmaxY);
        dmX = 200;
        nbinX = dmX * 2;
        dminX = -709 - dmX;
        dmaxX = -709 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[12] = new TH2D("d2_hist12_iQDS2_in", " d2_pos_iQDS2_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[13] = new TH2D("d2_hist13_iQDS3S_in", " d2_pos_iQDS3S_in ", nbinX, -960., -900., nbinY, dminY, dmaxY);
        d2_hist[14] = new TH2D("d2_hist14_iQDS4_in", " d2_pos_iQDS4_in ", nbinX, -1800., -1700., nbinY, dminY, dmaxY);
        dmX = 100;
        nbinX = dmX * 2;
        dminX = -dmX;
        dmaxX = dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[15] = new TH2D("d2_hist15_eBDS1_in", " d2_pos_eBDS1_in ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);

        d2_hist[20] = new TH2D("pt_at_RP", " pt at Roman pot", 100, 0.99, 1., 200, 0., 10.);

        //--------------------------------------------
        // ---- position at the  EXIT  of dipole -----
        dmX = 500;
        nbinX = dmX * 2;
        dminX = -333 - dmX;
        dmaxX = -333 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[24] = new TH2D("d2_hist24_iBDS1a_out", " d2_pos_iBDS1a_out ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[25] = new TH2D("d2_hist25_iBDS1b_out", " d2_pos_iBDS1b_out ", nbinX, -380., -320., nbinY, dminY, dmaxY);
        dmX = 400;
        nbinX = dmX * 2;
        dminX = -1368 - dmX;
        dmaxX = -1368 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[26] = new TH2D("d2_hist26_iBDS2_out", " d2_pos_iBDS2_out ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        dmX = 50;
        nbinX = dmX * 2 * 10;
        dminX = -1525 - dmX;
        dmaxX = -1525 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2 * 10;
        d2_hist[27] = new TH2D("d2_hist27_iBDS3_out", " d2_pos_iBDS3_out ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        // ---- position at the EXIT of quadr -----
        dmX = 100;
        nbinX = dmX * 2;
        dminX = -483 - dmX;
        dmaxX = -483 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[28] = new TH2D("d2_hist28_iQDS1a_out", " d2_pos_iQDS1a_out ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[29] = new TH2D("d2_hist29_iQDS1S_out", " d2_pos_iQDS1S_out ", nbinX, -550., -490., nbinY, dminY, dmaxY);
        dmX = 140;
        nbinX = dmX * 2;
        dminX = -658 - dmX;
        dmaxX = -658 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[30] = new TH2D("d2_hist30_iQDS1b_out", " d2_pos_iQDS1b_out ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[31] = new TH2D("d2_hist31_iQDS2S_out", " d2_pos_iQDS2S_out ", nbinX, -720., -660., nbinY, dminY, dmaxY);
        dmX = 200;
        nbinX = dmX * 2;
        dminX = -955 - dmX;
        dmaxX = -955 + dmX;
        dminY = -dmX;
        dmaxY = dmX;
        nbinY = dmX * 2;
        d2_hist[32] = new TH2D("d2_hist32_iQDS2_out", " d2_pos_iQDS2_out ", nbinX, dminX, dmaxX, nbinY, dminY, dmaxY);
        d2_hist[33] = new TH2D("d2_hist33_iQDS3S_out", " d2_pos_iQDS3S_out ", nbinX, -1180., -1120., nbinY, dminY, dmaxY);
        d2_hist[34] = new TH2D("d2_hist34_iQDS4_out", " d2_pos_iQDS4_out ", nbinX, -2100., -1700., nbinY, dminY, dmaxY);

        for (int in = 0; in < 12; in++) {
            sprintf(myname, "hmatrixOccup1_%d", in);
            hmatrixOccup[in] = new TH2F(myname, myname, NumCol, -0.5, NumCol - 0.5, NumRow, -0.5, NumRow - 0.5);
            sprintf(myname, "hmatrixOccupCM1_%d", in);
            hmatrixOccupCM[in] = new TH2F(myname, myname, 100, -10., 10., 100, -10., 10.);
        }

        for (int is = 0; is < detector->NannVAR; is++) {
            if (is < NHIST) {
                sprintf(Hname, "Likelihood:%02d", is);
                HLikelihood[is] = new TH1D(Hname, Hname, 100, emin, emax * (1 + detector->fadc_slice / 2.));
            }
        }


        gStyle->SetOptStat("nemruo");
        printf("booking done ! \n");


    }

    void FillGamDE(G4double de);  //--- fsv
    void FillGamAngle(G4double theta); //--- fsv
    void AddTrRef(G4double tr, G4double ref);

    void FillEn(G4double En);

    void FillTh(G4double Th);

    void FillThBack(G4double Th);

    void FillR(G4double R);

    void FillTt(G4double Tt);

    void FillTb(G4double Tt);

    void FillTsec(G4double T);

    void FillGammaInSpectrum(G4double E);

    void FillGammaOutSpectrum(G4double E);

    void FillGammaEStep(G4double E);

    void FillNbOfSteps(G4double nstep);

    void Fillvertexz(G4double z);



//    void SetRndmFreq(G4int val) { saveRndm = val; }

//    G4int GetRndmFreq() { return saveRndm; }


// #--------------------------------------------------------------------
// #----- HISTOGRAMS ---------------------------------------------------

    // Nb of bins in #step plot
    G4int nbinStep =    100;
    G4double Steplow =  0. ;
    G4double Stephigh = 10.;

    // Nb of bins in Edep plot
    int nbinEn =  100;
    double Enlow = 0.0 *MeV ;
    double Enhigh =  0.05 *MeV;

    // gamma spectrum plot
    int nbinGamma       = 500;
    double ElowGamma    = 0. *MeV;
    double EhighGamma   = 50.*MeV;

    // Etransmisssion plot
    int nbinTt    = 100;
    double Ttlow  = 0.      *MeV;
    double Tthigh = 10000.  *MeV;

    // Ebackscattered plot
    int nbinTb     = 100;
    double Tblow   = 0.      *MeV;
    double Tbhigh  = 10000.  *MeV;

    // Tsecondary  plot
    int nbinTsec;
    double Tseclow;
    double Tsechigh;

    // R plot
    int nbinR;
    double Rlow;
    double Rhigh;

    // Z plot
    int nbinvertexz;
    double zlow;
    double zhigh;


    // Nb of bins in Theta plot
    int nbinTh  = 100;
    double Thlow  =  89. * deg;
    double Thhigh  =  90. * deg;

    // Nb of bins in Theta plot
    G4int nbinThback;
    G4double Thhighback;
    G4double Thlowback;

#
#  histo2    Energy Loss (keV)
    /plots/setnbinEn
    /plots/setEnlow  MeV
    /plots/setEnhigh  MeV
#
#  histo3   angle distribution at exit(deg)
    /plots/setnbinTh
    /plots/setThlow
    /plots/setThhigh
#
#  histo4    lateral distribution at exit(mm)
    /plots/setnbinR  100
    /plots/setRlow   0.  mm
    /plots/setRhigh  400. mm

#
#  histo6    angle distribution of backscattered primaries(deg)
    /plots/setnbinThback  100
    /plots/setThlowback   0.   deg
    /plots/setThhighback  40. deg

#
#
#  histo8    kinetic energy of the charged secondaries (MeV)
    /plots/setnbinTsec  100
    /plots/setTseclow   0.   MeV
    /plots/setTsechigh  100.  MeV
#
#  histo9      z of secondary charged vertices(mm)
    /plots/setnbinzvertex  100
    /plots/setzlow   0.   mm
    /plots/setzhigh  100. mm



};

#endif //G4E_JLEICHISTOGRAMMING_HH
