#include "engine/CCombatSequence_BanditAttackSide.h"

struct CCharString {
    char* m_data;
    void Init(const char* s, int n);
};

struct CCombatSequence_BanditAttackSide_Methods : CCombatSequence_BanditAttackSide {
    CCharString* GetName(CCharString* result);
};

CCharString* CCombatSequence_BanditAttackSide_Methods::GetName(CCharString* result)
{
    result->Init((const char*)0x012787b8, -1);
    return result;
}
