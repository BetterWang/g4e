#ifndef G4E_ACTIONINITIALIZATION_HH
#define G4E_ACTIONINITIALIZATION_HH

#include <G4VUserActionInitialization.hh>
#include <root_output/RootOutputManager.hh>

class JLeicHistogramManager;

class JLeicActionInitialization: public G4VUserActionInitialization
{
public:
    JLeicActionInitialization(g4e::RootOutputManager *);

    void Build() const override ;
    // Virtual method to be implemented by the user to instantiate user action
    // class objects.

    /** Virtual method to be implemented by the user to instantiate user run action
    * class object to be used by G4MTRunManager. This method is not invoked in
    * the sequential mode. The user should not use this method to instantiate
    * user action classes rather than user run action.
    */
    void BuildForMaster() const override ;


private:
    g4e::RootOutputManager* mRootOuput;

};


#endif //G4E_ACTIONINITIALIZATION_HH
