// Empty member hook. __fastcall this=ecx.
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    void Hook();
};
void CResource_Methods::Hook() {}