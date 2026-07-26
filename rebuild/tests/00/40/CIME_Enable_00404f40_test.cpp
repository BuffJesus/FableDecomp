#include <cstdio>

struct CIME {
    unsigned char _pad_0x0[0x8];
    char enabled;
    unsigned char _pad_0x9[0x47];
    char flag50;
};

static int g_a_called = 0;
static int g_b_called = 0;
static char g_a_arg = -1;
static char g_b_arg = -1;

void __fastcall CIME_cb_a(char b) { g_a_called++; g_a_arg = b; }
void __fastcall CIME_cb_b(char b) { g_b_called++; g_b_arg = b; }

void __fastcall CIME_Enable(CIME* self, int edx, char enable)
{
    if (self->enabled) {
        CIME_cb_a(0);
        CIME_cb_b(enable != 0);
    }
}

int main()
{
    CIME o;
    o.enabled = 0;
    g_a_called = g_b_called = 0;
    CIME_Enable(&o, 0, 1);
    if (g_a_called != 0 || g_b_called != 0) { std::printf("FAIL disabled path\n"); return 1; }

    o.enabled = 1;
    g_a_called = g_b_called = 0; g_a_arg = g_b_arg = -1;
    CIME_Enable(&o, 0, 5);
    if (g_a_called != 1 || g_b_called != 1) { std::printf("FAIL enabled path calls\n"); return 1; }
    if (g_a_arg != 0) { std::printf("FAIL a arg\n"); return 1; }
    if (g_b_arg != 1) { std::printf("FAIL b arg\n"); return 1; }

    o.enabled = 1;
    g_a_called = g_b_called = 0; g_b_arg = -1;
    CIME_Enable(&o, 0, 0);
    if (g_b_arg != 0) { std::printf("FAIL b arg zero\n"); return 1; }

    std::printf("CIME_00404f40_TEST PASS\n");
    return 0;
}