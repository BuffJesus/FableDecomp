// Constant-false predicate: `xor al,al; ret`. __fastcall this=ecx, no args.
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    bool IsEditorActive();
};
bool CResource_Methods::IsEditorActive() { return false; }