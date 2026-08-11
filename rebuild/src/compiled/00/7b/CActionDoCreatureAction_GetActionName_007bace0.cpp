// CActionDoCreatureAction::GetActionName @ 0x007bace0
// __fastcall member returning CCharString by value (hidden retptr on stack, this in ecx, ret 4).

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    char pad;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123caf0, -1);
}