//
// Created by romanov on 11/24/19.
//

#ifndef G4E_JLEICHISTOGRAMMING_HH
#define G4E_JLEICHISTOGRAMMING_HH


#include <globals.hh>

class TH1D;
class TH2D;

class JLeicHistogramming {
public:

    TH1D *histo1, *histo2, *histo3, *histo4, *histo5, *histo6, *histo7, *histo8, *histo9, *histo10, *histo11, *histo12, *histo13, *histo14;
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
};

#endif //G4E_JLEICHISTOGRAMMING_HH
