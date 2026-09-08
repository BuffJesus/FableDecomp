#include "engine/CCombatSequenceWhisperIdles.h"

struct CCharString { char* m_data; CCharString(); CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequenceWhisperIdles_Methods : CCombatSequenceWhisperIdles { CCharString GetName(); };
CCharString CCombatSequenceWhisperIdles_Methods::GetName() { return CCharString((const char*)0x1278e38, -1); }
