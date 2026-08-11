// CActionDoCreatureAction::GetActionName  @ 0x007e1a80
// Struct-return (CCharString) factory: return CCharString("...literal...");
// Retail codegen:
//   push esi ; mov esi,[esp+8] ; push -1 ; push 0x123db28 ; mov ecx,esi
//   call 0x99ebf0 (CCharString ctor(const char*,int)) ; mov eax,esi ; pop esi ; ret 4

struct CCharString {
    // Real ctor at 0x99ebf0: CCharString::CCharString(const char* s, int len)
    // Declared as an ordinary member so VC7.1 emits: push len ; push s ; mov ecx,this ; call ctor.
    CCharString(const char* s, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

// Natural by-value struct return: VC7.1 passes the hidden return buffer as the first
// stack arg, 'this' in ecx, saves esi=buffer, invokes the ctor, returns buffer in eax.
CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123db28, -1);
}