#include "ci_TRD_Design.hh"
#include "ci_TRD_PhysicsConstructor.hh"
#include "ci_TRD_Messenger.hh"


ci_TRD_Design::ci_TRD_Design() {
    PhysicsConstructor = new ci_TRD_PhysicsConstructor(this);
    fMessenger = new ci_TRD_Messenger(this);
}
