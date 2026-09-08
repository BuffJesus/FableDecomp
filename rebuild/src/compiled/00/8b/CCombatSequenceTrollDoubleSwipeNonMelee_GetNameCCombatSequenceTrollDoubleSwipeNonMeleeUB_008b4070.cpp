#include "engine/CCombatSequence_TrollDoubleSwipeNonMelee.h"

struct CCharString { char* p; CCharString(const char*, int); };
struct CCombatSequence_TrollDoubleSwipeNonMelee_Methods : CCombatSequence_TrollDoubleSwipeNonMelee { CCharString GetName() const; };
CCharString CCombatSequence_TrollDoubleSwipeNonMelee_Methods::GetName() const { return CCharString((const char*)0x1278a18, -1); }
