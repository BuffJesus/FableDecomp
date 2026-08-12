// CActionDoCreatureAction::GetActionName  @ 008fe019
// Returns CCharString by value; the hidden return buffer arrives in ECX.
//
// Retail shape (19 bytes):
//   push esi ; push -1 ; push 0x128a07c ; mov esi,ecx ; call 0x99ebf0 ;
//   mov eax,esi ; pop esi ; ret
//
// For a __fastcall function that returns a small class by value, MSVC passes
// the hidden return-buffer pointer in ECX. The body constructs the constant
// name string into that slot (via the engine CCharString ctor @ 0x99ebf0 with
// length -1) and returns the slot in eax, with no stack cleanup (plain ret).

class CCharString
{
public:
    char* m_data;
    // engine ctor @ 0x99ebf0 : CCharString(const char*, int len) ; -1 = strlen
    CCharString(const char* s, int len);
};

class CActionDoCreatureAction { };

// this (unused) arrives via ecx per the original __fastcall; the CCharString
// return buffer is the fastcall struct-return slot.
CCharString __fastcall CActionDoCreatureAction_GetActionName(CActionDoCreatureAction* /*self*/)
{
    return CCharString("DoCreatureAction", -1);
}