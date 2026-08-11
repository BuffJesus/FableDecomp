// CActionDoCreatureAction::GetActionName @ 006285f0
// Real __fastcall member returning CCharString by value: this in ecx (unused),
// hidden UDT return pointer on the stack ([esp+8] after push esi), ret 4.
// Retail: push esi; mov esi,[esp+8]; push -1; push offset "..."; mov ecx,esi;
//         call CCharString::ctor(const char*,int); mov eax,esi; pop esi; ret 4
// The action name string lives at 0x123de20 (pushed as an immediate offset).

struct CCharString {
    int d;
    CCharString(const char* s, int n); // ctor @ 0x99ebf0
};

extern char kActionStr[];

struct CActionDoCreatureAction {
    int x;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionStr, -1);
}