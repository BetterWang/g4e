#ifndef G4E_INITIALIZATIONACTION_HH
#define G4E_INITIALIZATIONACTION_HH

#include <memory>

#include <G4VUserActionInitialization.hh>
#include <ArgumentProcessor.hh>


namespace g4e{
class ActionInitialization: public G4VUserActionInitialization
{
public:
    ActionInitialization()= default;


    void Build() const override ;
    // Virtual method to be implemented by the user to instantiate user action
    // class objects.

    /** Virtual method to be implemented by the user to instantiate user run action
    * class object to be used by G4MTRunManager. This method is not invoked in
    * the sequential mode. The user should not use this method to instantiate
    * user action classes rather than user run action.
    */
    void BuildForMaster() const override;

    void AddUserInitialization(G4VUserActionInitialization * init) { mActionInits.push_back(init); }

private:
    std::vector<G4VUserActionInitialization*> mActionInits;
};
}

#endif //G4E_INITIALIZATIONACTION_HH
