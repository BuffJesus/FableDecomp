#include "engine/CCombatSequence_FodderSecondaryAttack.h"

struct CCharString { char* m_buf; CCharString(const char*, int); };
static const char kName[] = "FodderSecondaryAttack";
struct CCombatSequence_FodderSecondaryAttack_Methods : CCombatSequence_FodderSecondaryAttack { CCharString GetName() const; };
CCharString CCombatSequence_FodderSecondaryAttack_Methods::GetName() const { return CCharString(kName, -1); }
