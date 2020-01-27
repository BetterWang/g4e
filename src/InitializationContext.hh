#ifndef G4E_INITIALIZATIONCONTEXT_HH
#define G4E_INITIALIZATIONCONTEXT_HH

#include <ArgumentProcessor.hh>
#include <MultiActionInitialization.hh>
#include <RootOutputManager.hh>

namespace g4e {
    struct InitializationContext {

        InitializationContext(UserArguments* args,
                              g4e::RootOutputManager* rootMan,
                              g4e::MultiActionInitialization* actionInit):
            Arguments(args),
            RootManager(rootMan),
            ActionInitialization(actionInit)
        {
        }

        UserArguments * Arguments;
        g4e::RootOutputManager * RootManager;
        g4e::MultiActionInitialization * ActionInitialization;
    };
}


#endif //G4E_INITIALIZATIONCONTEXT_HH
