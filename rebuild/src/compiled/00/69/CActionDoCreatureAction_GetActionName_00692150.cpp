#pragma optimize("g",on)

// Retail: value-returning __fastcall. ecx = CActionDoCreatureAction* (this, unused);
// hidden return-struct pointer arrives on the stack. Body constructs a CCharString
// from a string literal (0x125c0ec) via ctor 0x99ebf0 and returns it by value.
//   push esi; mov esi,[esp+8]; push -1; push 0x125c0ec; mov ecx,esi;
//   call 0x99ebf0; mov eax,esi; ret 4
// esi = hidden return slot, ecx set to it for the ctor thiscall.
struct CCharString {
    int dummy;
    CCharString(const char* lit, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c0ec, -1);
}