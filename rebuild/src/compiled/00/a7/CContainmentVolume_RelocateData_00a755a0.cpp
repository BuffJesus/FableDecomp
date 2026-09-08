#include "engine/CContainmentVolume.h"  // retyped onto the PDB layout; byte parity re-verified

void __fastcall RelocateData(
    CContainmentVolume* self, int /*edx*/, long a1,
    void* oldBase, void* newBase, long a4)
{
    (void)a1;
    (void)a4;
    if (self->ContainmentPlanes) {
        self->ContainmentPlanes = (CPlane*)((char*)self->ContainmentPlanes
            - (char*)oldBase + (char*)newBase);
    }
}
