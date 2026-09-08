#include "engine/CCombatSequenceWhisperBoast.h"

struct CCharString { char* m_pData; int m_nLength; CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequenceWhisperBoast_Methods : CCombatSequenceWhisperBoast { CCharString GetName() const; };
CCharString CCombatSequenceWhisperBoast_Methods::GetName() const { return CCharString(reinterpret_cast<const char*>(0x1278dc4), -1); }
