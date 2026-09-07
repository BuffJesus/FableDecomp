#include "engine/C3DMeshFileClothChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct C3DMeshFileClothChunk_Methods : C3DMeshFileClothChunk {
    CCharString GetStrikeEventHelperName();
};

CCharString C3DMeshFileClothChunk_Methods::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}