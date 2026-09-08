#include "engine/CCombatSequenceWhisperLunge.h"

struct CCharString { char* m_data; CCharString(); CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequenceWhisperLunge_Methods : CCombatSequenceWhisperLunge { CCharString GetName(); };
CCharString CCombatSequenceWhisperLunge_Methods::GetName() { return CCharString((const char*)0x1278ce0, -1); }
