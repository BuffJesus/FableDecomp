// CActionDoCreatureAction::GetActionName @ 0x0077bd00
// __fastcall, returns CCharString by value.

struct CCharString {
    char* p;
    int   len;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123bc4c, -1);
}