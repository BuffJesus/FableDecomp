// CCombatSequence_BanditKingBackOff::GetName @ 008b3b80
// Real __fastcall member returning CCharString by value: this in ecx (unused),
// hidden UDT return pointer on the stack ([esp+8] after push esi), ret 4.
// Retail: push esi; mov esi,[esp+8]; push -1; push offset "..."; mov ecx,esi;
//         call CCharString::ctor(const char*,int); mov eax,esi; pop esi; ret 4
// The name string lives at 0x12788b0 (pushed as an immediate offset).

struct CCharString {
    int d;
    CCharString(const char* s, int n); // ctor @ 0x99ebf0
};

extern char kNameStr[];

struct CCombatSequence_BanditKingBackOff {
    int x;
    CCharString GetName();
};

CCharString CCombatSequence_BanditKingBackOff::GetName()
{
    return CCharString(kNameStr, -1);
}