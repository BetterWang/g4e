#ifndef G4E_Materials_h
#define G4E_Materials_h

#include "globals.hh"

class G4Material;
namespace g4e {

class Materials
{
public:

    Materials();

    ~Materials();

    G4Material *GetMaterial(const G4String &);

private:

    void Initialise();
};

}
#endif  // G4E_Materials_h

 


