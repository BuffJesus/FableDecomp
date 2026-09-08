#include "engine/CCombatSequence_WillPhysicalShieldNonMelee.h"

struct CCharString { char* m_data; CCharString(); CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequence_WillPhysicalShieldNonMelee_Methods : CCombatSequence_WillPhysicalShieldNonMelee { CCharString GetName(); };
CCharString CCombatSequence_WillPhysicalShieldNonMelee_Methods::GetName() { return CCharString((const char*)0x1278fb8, -1); }
