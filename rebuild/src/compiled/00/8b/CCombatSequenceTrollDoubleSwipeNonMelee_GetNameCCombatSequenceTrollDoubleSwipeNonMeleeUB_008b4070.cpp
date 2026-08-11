// CCombatSequence_TrollDoubleSwipeNonMelee::GetName  (retail 0x008b4070)
// __fastcall, returns CCharString by value.
// Disasm: push esi; esi=hidden ret-buf; push -1; push LITERAL; mov ecx,esi;
//         call ctor; mov eax,esi; ret 4.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);   // @ 0x0099ebf0
};

struct CActionDoCreatureAction;

struct CCombatSequence_TrollDoubleSwipeNonMelee {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollDoubleSwipeNonMelee::GetName() const
{
    return CCharString((const char*)0x1278a18, -1);
}