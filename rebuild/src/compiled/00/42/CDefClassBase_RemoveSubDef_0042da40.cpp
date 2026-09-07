// Empty one-argument member: `ret 4`. __fastcall this=ecx, one stack arg.
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void SetInstantiationName(const char* name);
};
void CDefClassBase_Methods::SetInstantiationName(const char* name) {}