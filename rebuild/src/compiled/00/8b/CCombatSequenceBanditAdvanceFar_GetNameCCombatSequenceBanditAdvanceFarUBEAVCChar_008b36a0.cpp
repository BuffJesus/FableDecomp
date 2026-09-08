#include "engine/CCombatSequence_BanditAdvanceFar.h"

struct CCharString { char* p; CCharString(const char*, int); };
struct CCombatSequence_BanditAdvanceFar_Methods : CCombatSequence_BanditAdvanceFar { CCharString GetName() const; };
CCharString CCombatSequence_BanditAdvanceFar_Methods::GetName() const { return CCharString("CombatSequence_BanditAdvanceFar", -1); }
