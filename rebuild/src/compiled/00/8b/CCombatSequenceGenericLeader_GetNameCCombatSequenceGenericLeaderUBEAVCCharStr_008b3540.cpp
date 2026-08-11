// CCombatSequence_GenericLeader::GetName @ 008b3540
// __fastcall (modeled as member) returning CCharString by value:
// hidden retptr on stack, this in ecx, ret 4. Constructs a CCharString
// from the string literal at 0x12786d4 with length -1.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CCombatSequence_GenericLeader {
    CCharString GetName() const;
};

CCharString CCombatSequence_GenericLeader::GetName() const
{
    return CCharString((const char*)0x12786d4, -1);
}