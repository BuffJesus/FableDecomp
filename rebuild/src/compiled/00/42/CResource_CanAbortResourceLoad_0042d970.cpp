// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    bool Get(int a0);
};
bool CResource_Methods::Get(int a0) { return false; }