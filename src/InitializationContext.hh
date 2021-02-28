/***
 * This class holds information available/needed for detector/subdetector construction procedure
 */

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

        /// Holds information gained from user provided flags and G4E related environment variables
        /// This includes files to process, macro files, if it is batch job or GUI, multithreading
        UserArguments * Arguments;

        /// Holds information about output CERN ROOT file and helper classes that writes to the file
        g4e::RootOutputManager * RootManager;

        /// Allows to add UserActions EventAction, RunAction, etc.
        g4e::MultiActionInitialization * ActionInitialization;

        /// Global physics list
        EicPhysicsList *PhysicsList;
    };
}


#endif //G4E_INITIALIZATIONCONTEXT_HH
