#ifndef JLeicMaterials_h
#define JLeicMaterials_h

#include "globals.hh"

class G4Material;

class JLeicMaterials
{
public:

    JLeicMaterials();

    ~JLeicMaterials();

    G4Material *GetMaterial(const G4String &);

private:

    void Initialise();

};

#endif

 


