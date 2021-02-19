#include "ci_TRD_Design.hh"
#include "ci_TRDPhysics.hh"
#include "ci_TRDMessenger.hh"


ci_TRD_Design::ci_TRD_Design() {
    PhysicsConstructor = new ci_TRDPhysics(this);
    fMessenger = new ci_TRDMessenger(this);
}
