#include "engine/CPhysicsFacesSegmentBranch.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIPhysicsFacesSegmentBase {
    virtual ~CIPhysicsFacesSegmentBase();
    virtual long Sizeof();
};

struct CPhysicsFacesSegmentBranch_Methods : CPhysicsFacesSegmentBranch {
    // +0x18
    long Sizeof();
};

long CPhysicsFacesSegmentBranch_Methods::Sizeof() {
    return this->PositiveChild->Sizeof() + this->NegativeChild->Sizeof() + 0x1c;
}