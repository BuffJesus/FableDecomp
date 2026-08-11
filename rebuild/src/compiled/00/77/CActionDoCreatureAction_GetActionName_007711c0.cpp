// CActionDoCreatureAction::GetActionName @ 007711c0
// __fastcall, returns CCharString by value (hidden retptr on stack, ret 4)

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("SomeActionName", -1);
}