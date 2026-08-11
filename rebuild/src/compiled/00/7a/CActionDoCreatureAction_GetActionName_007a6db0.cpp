// CActionDoCreatureAction::GetActionName @ 007a6db0
// __fastcall, returns CCharString by value (hidden return ptr on stack).
// Body: construct CCharString from literal 0x123c49c with length -1.

class CCharString {
public:
    void* p;
    CCharString(const char* s, int len);
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

// literal string pointer baked in at 0x123c49c
static const char* const kName = (const char* const)0x0123c49c;

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kName, -1);
}