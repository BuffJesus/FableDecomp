#include <cstdio>

int g_someCount = 3;
float g_someFactor = 10.0f;

struct CTCCombat {
    char pad[0x68];
    int field_68;
};

char CTCCombat_IsVulnerableToUnblockableAttack(const CTCCombat* self)
{
    if ((float)self->field_68 > (float)g_someCount * g_someFactor)
        return 1;
    return 0;
}

int main()
{
    CTCCombat c;
    c.field_68 = 100;
    char r1 = CTCCombat_IsVulnerableToUnblockableAttack(&c); /* 100 > 30 -> 1 */
    c.field_68 = 10;
    char r2 = CTCCombat_IsVulnerableToUnblockableAttack(&c); /* 10 > 30 -> 0 */
    c.field_68 = 30;
    char r3 = CTCCombat_IsVulnerableToUnblockableAttack(&c); /* 30 > 30 -> 0 */
    if (r1 == 1 && r2 == 0 && r3 == 0) {
        std::printf("CTCCombat_007331c0_TEST PASS\n");
        return 0;
    }
    std::printf("CTCCombat_007331c0_TEST FAIL\n");
    return 1;
}