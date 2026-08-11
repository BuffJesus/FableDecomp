// CActionDoCreatureAction::GetActionName @ 00692250
// Returns a CCharString by value constructed from a literal string pointer.

struct CCharString {
    int d;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c168, -1);
}