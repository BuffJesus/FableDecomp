#include <cstdio>

struct CIME {
    char pad0[0x8];
    char enabled;     /* +0x8 */
    char pad9[0x47];  /* fill to 0x50 */
    char flag50;      /* +0x50 */
};

static int g_last = -1;
void __fastcall CIME_sink(bool b) { g_last = b ? 1 : 0; }

void __fastcall CIME_FinaliseString(CIME* self, void* /*edx*/, char param)
{
    if (self->enabled) {
        char v = param;
        if (param) {
            if (self->flag50)
                v = 0;
        }
        CIME_sink(v != 0);
    }
}

int main() {
    CIME c;
    /* enabled == 0 -> sink never called */
    c.enabled = 0; c.flag50 = 0; g_last = -1;
    CIME_FinaliseString(&c, 0, 1);
    if (g_last != -1) { std::printf("FAIL disabled\n"); return 1; }

    /* enabled, param==0 -> v=0 -> sink(false) */
    c.enabled = 1; c.flag50 = 0; g_last = -1;
    CIME_FinaliseString(&c, 0, 0);
    if (g_last != 0) { std::printf("FAIL p0\n"); return 1; }

    /* enabled, param!=0, flag50==0 -> v=param -> sink(true) */
    c.enabled = 1; c.flag50 = 0; g_last = -1;
    CIME_FinaliseString(&c, 0, 1);
    if (g_last != 1) { std::printf("FAIL p1f0\n"); return 1; }

    /* enabled, param!=0, flag50!=0 -> v=0 -> sink(false) */
    c.enabled = 1; c.flag50 = 1; g_last = -1;
    CIME_FinaliseString(&c, 0, 1);
    if (g_last != 0) { std::printf("FAIL p1f1\n"); return 1; }

    std::printf("CIME_00404b50_TEST PASS\n");
    return 0;
}