#include "engine/CCombatSequence_ScorpionLungeAttack.h"

struct CCharString { char* m_data; CCharString(const char*, long); };
extern const char kScorpionLungeName[];
struct CCombatSequence_ScorpionLungeAttack_Methods : CCombatSequence_ScorpionLungeAttack { CCharString GetName() const; };
CCharString CCombatSequence_ScorpionLungeAttack_Methods::GetName() const { return CCharString(kScorpionLungeName, -1); }
