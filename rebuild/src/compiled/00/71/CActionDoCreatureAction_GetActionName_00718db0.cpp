// CActionDoCreatureAction::GetActionName  @ 00718db0
// Native __fastcall member: this in ECX (ignored), struct return via hidden
// stack pointer, ret 4. Constructs CCharString(0x1265850, -1) into the slot.

struct CCharString {
    char* buf;
    CCharString(const char* s, int n);
    CCharString(const CCharString&);   // forces return-in-memory ABI
};

struct CActionDoCreatureAction {
    int _pad;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x1265850, -1);
}