#ifndef JLeicCe_emcalDigiHit_h
#define JLeicCe_emcalDigiHit_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "root_output/RootFlatIO.hh"
#include "G4RunManager.hh"

#include <iostream>

#include "JLeicCe_emcalHit.hh"
using namespace std;

class JLeicCe_emcalDigiHit
{
public:

    JLeicCe_emcalDigiHit();

   ~JLeicCe_emcalDigiHit() ;//override;

    JLeicCe_emcalDigiHit(const JLeicCe_emcalDigiHit &);

   JLeicCe_emcalDigiHit*  JLeicCe_emcalDigi(JLeicCe_emcalHit*);
  JLeicCe_emcalDigiHit(int);

    void *operator new(size_t);

    void operator delete(void *);

    const JLeicCe_emcalDigiHit &operator=(const JLeicCe_emcalDigiHit &);

    void Print();

    double BirksAttenuation(double,double,int,double);
    double* WaveFormPbwo(double,double*);


    inline void SetDetName(string dname)           { Pdetector_name = dname; }
    inline string GetDetName()                     { return  Pdetector_name; }

    inline void SetEdep(double edep)               { PEdep_tot = edep; }
    inline double GetEdep()                        { return  PEdep_tot; }

    inline void SetNpe(int npe)                   { Pnpe = npe; }
    inline double GetNpe()                        { return  Pnpe; }

    inline void SetADC(double ADC)                 { PADC_crs = ADC; }
    inline double GetADC()                         { return  PADC_crs; }

    inline void SetTDC(double TDC)                 { PTDC_crs = TDC; }
    inline double GetTDC()                         { return  PTDC_crs; }

 //   inline void Setwaveform( vector<double> waveform){ Pwaveform = waveform; }
  //  inline  vector<double> GetWaveform()             { return  Pwaveform; }


private:

//g4e::RootOutputManager* mRootEventsOut = nullptr;
   string Pdetector_name;    //< detector name
   double PEdep_tot;
   int Pnpe;
   double PADC_crs;
   double PTDC_crs;
 //  vector<double> Pwaveform;


};


//typedef G4TDigiCollection<JLeicCe_emcalDigiHit> JLeicCe_emcalDigiHitCollection;

extern G4ThreadLocal G4Allocator<JLeicCe_emcalDigiHit> *JLeicCe_emcalDigiHitAllocator;


inline void *JLeicCe_emcalDigiHit::operator new(size_t)
{
    void *aHit;
    if (!JLeicCe_emcalDigiHitAllocator) JLeicCe_emcalDigiHitAllocator = new G4Allocator<JLeicCe_emcalDigiHit>();
    aHit = (void *) JLeicCe_emcalDigiHitAllocator->MallocSingle();
    return aHit;
}


inline void JLeicCe_emcalDigiHit::operator delete(void *aHit)
{
    if (!JLeicCe_emcalDigiHitAllocator) JLeicCe_emcalDigiHitAllocator = new G4Allocator<JLeicCe_emcalDigiHit>();
    JLeicCe_emcalDigiHitAllocator->FreeSingle((JLeicCe_emcalDigiHit *) aHit);
}

#endif


