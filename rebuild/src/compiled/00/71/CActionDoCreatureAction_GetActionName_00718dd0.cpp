// CActionDoCreatureAction::GetActionName @ 0x00718dd0
// __fastcall returning CCharString by value (hidden return ptr).
// Body: CCharString(0x1265884, -1); this (ecx) unused.

class CCharString {
public:
    char* m_p;
    CCharString(const char* s, int n);
};

class CActionDoCreatureAction {
public:
    int dummy;
    CCharString GetActionName();
};

// member => this in ecx, hidden ret ptr on stack => ret 4
CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x1265884, -1);
}