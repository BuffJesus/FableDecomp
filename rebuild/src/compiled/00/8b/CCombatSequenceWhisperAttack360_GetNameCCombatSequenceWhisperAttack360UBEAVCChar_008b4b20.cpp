// Faithful VC7.1 reconstruction of
//   ?GetName@CCombatSequenceWhisperAttack360@@UBE?AVCCharString@@XZ  (0x008b4b20)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return buffer (CCharString*)
//   push -1
//   push 0x1278d48           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, int)  ; 0x99ebf0
//   mov  eax, esi
//   pop  esi
//   ret  4
//
// A GetName() const returning a CCharString by value, NRVO'd into the
// caller-supplied return buffer, initialised from a string literal with an
// explicit length arg of -1.

class CCharString
{
public:
    // Declaration only: defined in another TU (retail 0x99ebf0), so the
    // compiler MUST emit a real call.
    CCharString(const char* s, int len);
private:
    char* m_data;
};

class CCombatSequenceWhisperAttack360
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequenceWhisperAttack360::GetName() const
{
    return CCharString((const char*)0x1278d48, -1);
}