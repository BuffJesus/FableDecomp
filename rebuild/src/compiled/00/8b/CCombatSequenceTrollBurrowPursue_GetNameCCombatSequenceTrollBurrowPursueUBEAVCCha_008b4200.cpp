#include "engine/CCombatSequence_TrollBurrowPursue.h"

struct CCharString { char* m_buf; CCharString(const char*, int); };
static const char kName[] = "TrollBurrowPursue";
struct CCombatSequence_TrollBurrowPursue_Methods : CCombatSequence_TrollBurrowPursue { CCharString GetName() const; };
CCharString CCombatSequence_TrollBurrowPursue_Methods::GetName() const { return CCharString(kName, -1); }
