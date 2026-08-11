// CCombatSequence_FodderUnblockableMainAttack::GetName
// retail 0x008b3d00 : __fastcall CCharString GetName(CActionDoCreatureAction*)
//
// push esi
// mov esi,[esp+8]          ; esi = hidden struct-return buffer (CCharString*)
// push -1
// push 0x1278924          ; const char* literal
// mov ecx,esi
// call 0x99ebf0           ; CCharString::CCharString(this=esi, const char*, int=-1)
// mov eax,esi
// pop esi
// ret 4                    ; clean the return-buffer stack arg only

class CCharString {
public:
    // ctor at 0x99ebf0 : CCharString(const char*, int)
    CCharString(const char* s, int n);
};

// ecx=self (the sequence object), plus the by-value return buffer + unused param.
// Model as a free __fastcall so ecx=self; return CCharString by value so the
// compiler passes the hidden return-buffer ptr and emits the ret 4.
class CCombatSequence_FodderUnblockableMainAttack {
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_FodderUnblockableMainAttack::GetName() const
{
    return CCharString((const char*)0x1278924, -1);
}