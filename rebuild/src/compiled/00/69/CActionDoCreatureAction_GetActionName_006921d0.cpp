struct CCharString {
    int d;
    CCharString(const char* lit, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

// __fastcall member returning CCharString by value: this in ecx, hidden return
// ptr on stack, ret 4. this is unused.
CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c130, -1);
}