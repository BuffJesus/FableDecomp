#include "engine/CCombatSequence_VillagerBackOff.h"

struct CCharString { char* m_pData; CCharString(const char*, int); };
struct CCombatSequence_VillagerBackOff_Methods : CCombatSequence_VillagerBackOff { CCharString GetName() const; };
CCharString CCombatSequence_VillagerBackOff_Methods::GetName() const { return CCharString("CombatSequence_VillagerBackOff", -1); }
