// Byte-exact reconstruction of CCombatSequence_ScorpionLungeAttack::IsValid @ 0x00906fa0
struct CCombatSequence_ScorpionLungeAttack {
    char pad[0x28];
    int field28; // +0x28
};

extern int g_globalMultiplier;                 // ds:0x1375550
extern "C" int __cdecl GetCurrentValue();      // 0x49d870

bool __fastcall IsValid(CCombatSequence_ScorpionLungeAttack* self)
{
    int mult = g_globalMultiplier;
    if (GetCurrentValue() - self->field28 > mult * 10)
        return true;
    return false;
}