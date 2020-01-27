#ifndef G4E_INITIALIZATIONACTION_HH
#define G4E_INITIALIZATIONACTION_HH

#include <memory>

#include <G4VUserActionInitialization.hh>
#include <G4UserEventAction.hh>
#include <ArgumentProcessor.hh>


namespace g4e{
class MultiActionInitialization: public G4VUserActionInitialization
{
public:
    MultiActionInitialization()= default;


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

    void AddUserActionGenerator(const std::function<G4UserEventAction*()>& generator) {
        mEventActionGenerators.push_back(generator);
    }

    void AddUserActionGenerator(const std::function<G4UserRunAction*()>& generator) {
        mRunActionGenerators.push_back(generator);
    }

private:
    std::vector<G4VUserActionInitialization*> mActionInits;
    std::vector<std::function<G4UserEventAction*()>> mEventActionGenerators;
    std::vector<std::function<G4UserRunAction*()>> mRunActionGenerators;
};
}

#endif //G4E_INITIALIZATIONACTION_HH
