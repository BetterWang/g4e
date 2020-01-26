#ifndef G4E_INITIALIZATIONCONTEXT_HH
#define G4E_INITIALIZATIONCONTEXT_HH

#include <ArgumentProcessor.hh>
#include <ActionInitialization.hh>
#include <RootOutputManager.hh>

namespace g4e {
    struct InitializationContext {
        UserArguments Arguments;
        g4e::RootOutputManager* RootOutput;
        g4e::ActionInitialization* ActionInitialization;
    };
}


#endif //G4E_INITIALIZATIONCONTEXT_HH
