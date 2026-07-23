extern int g_someCount;
extern float g_someFactor;

struct CTCCombat {
    char pad[0x68];
    int field_68;
};

char __fastcall CTCCombat_IsVulnerableToUnblockableAttack(const CTCCombat* self)
{
    if ((float)self->field_68 > (float)g_someCount * g_someFactor)
        return 1;
    return 0;
}