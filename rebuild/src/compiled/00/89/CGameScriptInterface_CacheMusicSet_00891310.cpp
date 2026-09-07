#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum EMusicSetType { EMS_A };

struct CMusicTarget
{
    void CacheMusicSet(EMusicSetType t);
};

struct CWorld
{
    char pad_0000[0x6C];
    CMusicTarget* target_006C;
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void CacheMusicSet(EMusicSetType t) const;
};

void CGameScriptInterface_Methods::CacheMusicSet(EMusicSetType t) const
{
    World->target_006C->CacheMusicSet(t);
}