// `return this->Index != 0;` (xor/test/setne). __fastcall this=ecx.
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CWADFile_Methods : CWADFile {
    bool IsSet();
};
#pragma pack(pop)
bool CWADFile_Methods::IsSet() { return this->Index != 0; }