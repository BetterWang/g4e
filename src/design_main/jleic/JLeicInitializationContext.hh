//
// Created by romanov on 11/25/19.
//

#ifndef G4E_JLEICINITIALIZATIONCONTEXT_HH
#define G4E_JLEICINITIALIZATIONCONTEXT_HH

#include <InitializationContextInterface.hh>
#include <InitializationMConfig.hh>
#include <LogLevels.hh>

class JLeicInitializationContext: public InitializationContextInterface {
public:

    JLeicInitializationContext(InitializationMConfig* config): config(fConfig) {

    }

    LogLevels GetLogLevel() { return fConfig->GetLogLevel(); }

private:

    InitializationMConfig *fConfig;

};

#endif //G4E_JLEICINITIALIZATIONCONTEXT_HH
