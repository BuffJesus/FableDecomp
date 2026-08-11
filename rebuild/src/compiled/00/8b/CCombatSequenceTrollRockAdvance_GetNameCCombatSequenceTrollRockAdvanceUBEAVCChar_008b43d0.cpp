// Faithful VC7.1 reconstruction of
//   ?GetName@CCombatSequence_TrollRockAdvance@@UBE?AVCCharString@@XZ  (0x008b43d0)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return buffer (CCharString*)
//   push -1
//   push 0x1278b3c           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, int)  ; 0x99ebf0
//   mov  eax, esi
//   pop  esi
//   ret  4
//
// A GetName() const returning CCharString by value, NRVO'd into the
// caller-supplied return buffer, initialised from a string literal with an
// explicit length arg of -1.

class CCharString
{
public:
    // Declaration only: defined in another TU (retail 0x99ebf0), so the
    // compiler MUST emit a real call, matching retail's push/push/call.
    CCharString(const char* s, int len);
private:
    char* m_data;
};

class CActionDoCreatureAction;

class CCombatSequence_TrollRockAdvance
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockAdvance::GetName() const
{
    return CCharString("SEQUENCE_TROLL_ROCK_ADVANCE", -1);
}