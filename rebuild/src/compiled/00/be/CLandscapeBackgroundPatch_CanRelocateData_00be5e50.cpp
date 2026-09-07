#include "engine/CLandscapeBackgroundPatch.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner { char pad[0x5c]; unsigned char flag; };
struct CLandscapeBackgroundTreeNode { char pad[0xa4]; Inner* inner; };

bool __fastcall CanRelocateData(CLandscapeBackgroundPatch* self, int /*edx*/, long a) {
    (void)a;
    return self->Parent->inner->flag == 0;
}