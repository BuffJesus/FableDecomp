// CCombatSequence_VillagerComboUnblockable::GetName  @ 008b4820
// returns CCharString by value; hidden return buffer ptr is a stack arg.

struct CCharString {
    char* p;
    // CCharString::CCharString(char const*, int)  @ 0x99ebf0  (__fastcall)
    void ctor(const char* s, int n);
};

struct CCombatSequence_VillagerComboUnblockable {
    void* vtbl;
};

// __fastcall: self in ecx, hidden return-buffer ptr is first stack arg.
// Model as free __fastcall: (self=ecx, edx, retbuf).
extern "C" CCharString* __fastcall CCombatSequence_VillagerComboUnblockable_GetName(
    CCombatSequence_VillagerComboUnblockable* self, int edx, CCharString* ret)
{
    ret->ctor("VillagerComboUnblockable", -1);
    return ret;
}