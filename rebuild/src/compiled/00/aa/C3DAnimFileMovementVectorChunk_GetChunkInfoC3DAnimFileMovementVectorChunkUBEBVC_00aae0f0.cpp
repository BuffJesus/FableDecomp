#include "engine/C3DAnimFileMovementVectorChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct C3DAnimFileMovementVectorChunk_Methods : C3DAnimFileMovementVectorChunk {
    CCharString GetStrikeEventHelperName();
};

CCharString C3DAnimFileMovementVectorChunk_Methods::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}