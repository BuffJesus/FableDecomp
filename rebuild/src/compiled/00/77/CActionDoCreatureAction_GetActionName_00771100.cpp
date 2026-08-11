// CActionDoCreatureAction::GetActionName @ 0x00771100
// Retail: push esi; mov esi,[esp+8]; push -1; push 0x123b4b4; mov ecx,esi;
//         call CCharString::ctor(this=buf, s, len); mov eax,esi; pop esi; ret 4
// __fastcall member returning CCharString by value: this in ecx, hidden ret ptr
// on stack ([esp+4] before push esi -> [esp+8] after). Constructs a CCharString
// from a string literal at abs 0x123b4b4 with length -1, into the return slot.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

// The string literal lives at absolute 0x123b4b4 in retail .rdata.
extern const char kActionNameLiteral[];

struct CActionDoCreatureAction {
    char dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionNameLiteral, -1);
}