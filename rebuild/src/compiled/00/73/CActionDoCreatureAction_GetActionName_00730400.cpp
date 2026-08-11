// CActionDoCreatureAction::GetActionName @ 0x00730400
// __fastcall, returns CCharString by value (hidden return pointer).
// Body: construct CCharString from literal string ptr 0x123a680 with -1.

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123a680, -1);
}