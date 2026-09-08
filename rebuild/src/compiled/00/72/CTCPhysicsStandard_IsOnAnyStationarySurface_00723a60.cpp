#include "engine/CTCPhysicsStandard.h"  // retyped onto the PDB layout; byte parity re-verified

struct CStationarySurfaceView {
    unsigned char _pad_0x00[0x3c];
    unsigned char Flags;
};
struct CPhysicsOwnerObjectView {
    unsigned char _pad_0x00[0x60];
    CStationarySurfaceView* Surface;
};
struct CPhysicsOwnerView;
struct CPhysicsOwnerVTable {
    unsigned char _pad_0x00[0x98];
    CPhysicsOwnerObjectView* (__fastcall* GetPhysicsObject)(CPhysicsOwnerView*);
};
struct CPhysicsOwnerView { CPhysicsOwnerVTable* __vftable; };
struct CStationarySurfaceVTable {
    unsigned char _pad_0x00[0xd0];
    bool (__fastcall* IsStationary)(CStationarySurfaceView*);
};
struct CTCPhysicsStandardVTable {
    unsigned char _pad_0x00[0xdc];
    bool (__fastcall* HasStationarySurface)(CTCPhysicsStandard*);
    bool (__fastcall* IsAlwaysOnSurface)(CTCPhysicsStandard*);
};

bool __fastcall CTCPhysicsStandard_IsOnAnyStationarySurface(
    CTCPhysicsStandard* self)
{
    if (((CTCPhysicsStandardVTable*)self->__vftable)->IsAlwaysOnSurface(self))
        return true;
    if (((CTCPhysicsStandardVTable*)self->__vftable)->HasStationarySurface(self)) {
        CPhysicsOwnerView* owner = (CPhysicsOwnerView*)self->owner;
        CPhysicsOwnerObjectView* physics =
            owner->__vftable->GetPhysicsObject(owner);
        if (physics) {
            CStationarySurfaceView* surface = physics->Surface;
            if (surface) {
                if (((CStationarySurfaceVTable*)*(void**)surface)->IsStationary(surface))
                    return true;
                if (physics->Surface->Flags & 1)
                    return false;
            }
        }
        return true;
    }
    return false;
}
