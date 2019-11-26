//
// Created by romanov on 11/25/19.
//

#ifndef G4E_INITIALIZATIONCONTEXTINTERFACE_HH
#define G4E_INITIALIZATIONCONTEXTINTERFACE_HH


#include "LogLevels.hh"

class InitializationContextInterface {
public:

    virtual LogLevels GetLogLevel() = 0;
};

#endif //G4E_INITIALIZATIONCONTEXTINTERFACE_HH
