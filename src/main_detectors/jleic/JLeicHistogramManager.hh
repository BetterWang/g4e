//
// Created by romanov on 11/24/19.
//

#ifndef G4E_JLEICHISTOGRAMMANAGER_HH
#define G4E_JLEICHISTOGRAMMANAGER_HH

#include <globals.hh>
#include <G4SystemOfUnits.hh>


class TH1D;

class TH2D;

class TH2F;

class JLeicHistogramManager
{
public:

    TH1D *histo1 = nullptr;
    TH1D *histo2 = nullptr;
    TH1D *histo3 = nullptr;
    TH1D *histo4 = nullptr;
    TH1D *histo5 = nullptr;
    TH1D *histo6 = nullptr;
    TH1D *histo7 = nullptr;
    TH1D *histo8 = nullptr;
    TH1D *histo9 = nullptr;
    TH1D *histo10 = nullptr;
    TH1D *histo11 = nullptr;
    TH1D *histo12 = nullptr;
    TH1D *histo13 = nullptr;
    TH1D *histo14 = nullptr;
    TH1D *histo15 = nullptr;
    TH1D *histo16 = nullptr;

    //static TH2D* d2_pos_ffq1;
    static const int NHIST = 50;
    TH1D *hist[NHIST];
    TH1D *HLikelihood[NHIST];
    TH2D *d2_hist[NHIST];

    int NumRow = 10;
    int NumCol = 50;
    TH2F *hmatrixOccup[120];
    TH2F *hmatrixOccupCM[120];

    JLeicHistogramManager();

    void Write();



//    void FillGamDE(G4double de);  //--- fsv
//    void FillGamAngle(G4double theta); //--- fsv
//    void AddTrRef(G4double tr, G4double ref);

    void FillHLikelihood(int ihist, G4double val);

    void FillHist(int ihist, G4double val);

    void FillHist(int ihist, G4double val, G4double w);

    void FillHist(int ihist, int bin, G4double w);

    void ResetHist(int ihist);

//---  2D ---
    void FillHist2d(int ihist, G4double valx, G4double valy, G4double w);

    G4double GetHist2d(int ihist, G4int ix, G4int iy);

    void ResetHist2d(int ihist);

    void FillHistmatrixOccup(int ihist, G4double valx, G4double valy, G4double w);

    void FillHistmatrixOccupCM(int ihist, G4double valx, G4double valy, G4double w);


// #--------------------------------------------------------------------
// #----- HISTOGRAMS ---------------------------------------------------

    // Nb of bins in #step plot
    G4int nbinStep = 100;
    G4double Steplow = 0.;
    G4double Stephigh = 10.;

    // Nb of bins in Edep plot
    int nbinEn = 100;
    double Enlow = 0.0 * MeV;
    double Enhigh = 0.05 * MeV;

    // gamma spectrum plot
    int nbinGamma = 500;
    double ElowGamma = 0. * MeV;
    double EhighGamma = 50. * MeV;

    // Etransmisssion plot
    int nbinTt = 100;
    double Ttlow = 0. * MeV;
    double Tthigh = 10000. * MeV;

    // Ebackscattered plot
    int nbinTb = 100;
    double Tblow = 0. * MeV;
    double Tbhigh = 10000. * MeV;

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
    int nbinTh = 100;
    double Thlow = 89. * deg;
    double Thhigh = 90. * deg;

    // Nb of bins in Theta plot
    G4int nbinThback;
    G4double Thhighback;
    G4double Thlowback;

    void Book();
};

#endif //G4E_JLEICHISTOGRAMMANAGER_HH
