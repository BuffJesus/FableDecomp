#include "engine/CCombatSequence_WillPhysicalShield.h"

struct CCharString { char* m_data; CCharString(); CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequence_WillPhysicalShield_Methods : CCombatSequence_WillPhysicalShield { CCharString GetName(); };
CCharString CCombatSequence_WillPhysicalShield_Methods::GetName() { return CCharString((const char*)0x1278f90, -1); }
