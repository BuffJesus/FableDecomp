#include "engine/CCombatSequenceWhisperAttackBigCombo.h"

struct CCharString { char* m_data; CCharString(); CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequenceWhisperAttackBigCombo_Methods : CCombatSequenceWhisperAttackBigCombo { CCharString GetName(); };
CCharString CCombatSequenceWhisperAttackBigCombo_Methods::GetName() { return CCharString((const char*)0x1278d20, -1); }
