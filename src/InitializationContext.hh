#ifndef G4E_INITIALIZATIONCONTEXT_HH
#define G4E_INITIALIZATIONCONTEXT_HH

#include <ArgumentProcessor.hh>
#include <MultiActionInitialization.hh>
#include <RootOutputManager.hh>
#include <EicPhysicsList.hh>

namespace g4e {
    struct InitializationContext {

        InitializationContext(UserArguments* args,
                              g4e::RootOutputManager* rootMan,
                              g4e::MultiActionInitialization* actionInit,
                              EicPhysicsList* physicsList):
            Arguments(args),
            RootManager(rootMan),
            ActionInitialization(actionInit),
            PhysicsList(physicsList)
        {
        }

        UserArguments * Arguments;
        g4e::RootOutputManager * RootManager;
        g4e::MultiActionInitialization * ActionInitialization;
        EicPhysicsList *PhysicsList;
    };
}


#endif //G4E_INITIALIZATIONCONTEXT_HH
