#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct CCreatureActionBase_Methods : CCreatureActionBase {
    CCharString GetStrikeEventHelperName();
};

CCharString CCreatureActionBase_Methods::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}