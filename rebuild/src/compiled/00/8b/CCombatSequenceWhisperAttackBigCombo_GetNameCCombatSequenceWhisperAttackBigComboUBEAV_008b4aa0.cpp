// CCombatSequenceWhisperAttackBigCombo::GetName  @ retail 0x008b4aa0
// Returns a CCharString by value built from a fixed string literal pointer.
//
// push esi
// mov esi,[esp+8]          ; esi = hidden struct-return buffer (CCharString*)
// push -1
// push 0x1278d20           ; const char* literal
// mov ecx,esi
// call 0x99ebf0            ; CCharString::CCharString(this=esi, const char*, int=-1)
// mov eax,esi
// pop esi
// ret 4                    ; clean the return-buffer stack arg only

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequenceWhisperAttackBigCombo {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperAttackBigCombo::GetName()
{
    return CCharString((const char*)0x1278d20, -1);
}