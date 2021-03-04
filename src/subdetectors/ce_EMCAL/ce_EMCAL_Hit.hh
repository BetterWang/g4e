#ifndef JLeicCe_emcalHit_h
#define JLeicCe_emcalHit_h 1


#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include <iostream>
using namespace std;

class ce_EMCAL_Hit : public G4VHit
{
public:

    ce_EMCAL_Hit();

   ~ce_EMCAL_Hit() override;

    ce_EMCAL_Hit(const ce_EMCAL_Hit &);

    void *operator new(size_t);

    void operator delete(void *);

    const ce_EMCAL_Hit &operator=(const ce_EMCAL_Hit &);

    void Print();

public:

    void AddAbs(G4double de, G4double dl)
    {
        fEdepAbs += de;
        fTrackLengthAbs += dl;
    };

    void AddGap(G4double de, G4double dl)
    {
        fEdepGap += de;
        fTrackLengthGap += dl;
    };

    G4double GetEdepAbs() { return fEdepAbs; };

    G4double GetTrakAbs() { return fTrackLengthAbs; };

    G4double GetEdepGap() { return fEdepGap; };

    G4double GetTrakGap() { return fTrackLengthGap; };

private:

    G4double fEdepAbs, fTrackLengthAbs;
    G4double fEdepGap, fTrackLengthGap;

    std::vector<G4ThreeVector>   pos;    /// Hit Position (Global)
    std::vector<G4ThreeVector>  Lpos;    /// Hit Positions (Local to the Volume)
    std::vector<double>         edep;    /// Energy Deposited
    std::vector<double>           dx;    /// Length of the step
    vector<int>                    q;    /// Charge of the particle in each step
    vector<int>                  pid;    /// PID
    std::vector<double>         time;    /// Time from the start of event
    std::vector<string> materialName;    /// Material name
    string detector_name;                /// detector name (cell name)
    G4int mCopyNumber;                   /// detector copy number
    double                 XHalfLength_det;  /// XHalfLength of crystal
    double                 YHalfLength_det;  /// YHalfLength of crystal
    double                 ZHalfLength_det;  /// ZHalfLength of crystal

    double   X_crs;  /// X of crystal
    double   Y_crs;  /// YHalfLength of crystal
    double   Z_crs;  /// ZHalfLength of crystal


public:

    inline void SetPos(G4ThreeVector xyz)       { pos.push_back(xyz); }
    inline vector<G4ThreeVector> GetPos()       { return pos; }

    inline void SetLPos(G4ThreeVector xyz)      { Lpos.push_back(xyz); }
    inline vector<G4ThreeVector> GetLPos()      { return Lpos; }

    inline void SetEdep(double depe)            { edep.push_back(depe); }
    inline vector<double> GetEdep()             { return edep; }

    inline void SetDx(double Dx)                { dx.push_back(Dx); }
    inline vector<double> GetDx()               { return dx; }

    inline void SetTime(double ctime)           { time.push_back(ctime); }
    inline vector<double> GetTime()             { return  time; }

    inline void SetMatName(string mname)           { materialName.push_back(mname); }
    inline void SetMatNames(vector<string> mnames) { materialName = mnames; }
    inline string GetMatName()                     { return  materialName[0]; }
    inline vector<string> GetMatNames()            { return  materialName; }

    inline void SetDetName(string dname)           { detector_name = dname; }
    inline string GetDetName()                     { return  detector_name; }

    inline void SetCharge(int Q)                { q.push_back(Q); }
    inline int GetCharge()                      { return q[0]; }
    inline vector<int> GetCharges()             { return q; }

    inline void SetPid(int PID)                { pid.push_back(PID); }
    inline int GetPid()                      { return pid[0]; }
    inline vector<int> GetPids()             { return pid; }

    inline void SetXHalfLength(double xhalf)            { XHalfLength_det=xhalf; }
    inline double GetXHalfLength()              { return XHalfLength_det; }

    inline void SetYHalfLength(double yhalf)            { YHalfLength_det=yhalf; }
    inline double GetYHalfLength()              { return YHalfLength_det; }

    inline void SetZHalfLength(double zhalf)            { ZHalfLength_det=zhalf; }
    inline double GetZHalfLength()              { return ZHalfLength_det; }

    inline void SetX_crs(double xcrs)                   { X_crs=xcrs; }
    inline double GetX_crs()                    { return X_crs; }

    inline void SetY_crs(double ycrs)                   { Y_crs=ycrs; }
    inline double GetY_crs()                    { return Y_crs; }

    inline void SetZ_crs(double zcrs)                   { Z_crs=zcrs; }
    inline double GetZ_crs()                    { return Z_crs; }

    void SetCopyNumber(G4int copyNumber) { mCopyNumber = copyNumber; }
    G4int GetCopyNumber() {return mCopyNumber;}
};


typedef G4THitsCollection<ce_EMCAL_Hit> ce_EMCAL_HitsCollection;

extern G4ThreadLocal G4Allocator<ce_EMCAL_Hit> *ce_EMCAL_HitAllocator;


inline void *ce_EMCAL_Hit::operator new(size_t)
{
    void *aHit;
    if (!ce_EMCAL_HitAllocator) ce_EMCAL_HitAllocator = new G4Allocator<ce_EMCAL_Hit>();
    aHit = (void *) ce_EMCAL_HitAllocator->MallocSingle();
    return aHit;
}


inline void ce_EMCAL_Hit::operator delete(void *aHit)
{
    if (!ce_EMCAL_HitAllocator) ce_EMCAL_HitAllocator = new G4Allocator<ce_EMCAL_Hit>();
    ce_EMCAL_HitAllocator->FreeSingle((ce_EMCAL_Hit *) aHit);
}

#endif


