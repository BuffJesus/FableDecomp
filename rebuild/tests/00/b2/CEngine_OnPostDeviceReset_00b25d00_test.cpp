#include <cstdio>

struct B { int r; };

static int g_reset_ret;
static B* g_last_base;

int __fastcall B_DoReset(B* self) {
    g_last_base = self;
    return g_reset_ret;
}

struct CEngine {
    char pad1[0xb1];
    unsigned char flag_b1;
};

bool __fastcall CEngine_OnPostDeviceReset(CEngine* self) {
    if (self->flag_b1) {
        B* base = (B*)((char*)self - 0xc);
        return B_DoReset(base) != 2;
    }
    return true;
}

int main() {
    CEngine e;
    // Case 1: flag clear -> true, no call
    e.flag_b1 = 0;
    g_last_base = 0;
    if (CEngine_OnPostDeviceReset(&e) != true) { printf("FAIL flag0\n"); return 1; }
    if (g_last_base != 0) { printf("FAIL called when flag clear\n"); return 1; }

    // Case 2: flag set, reset returns 2 -> false
    e.flag_b1 = 1;
    g_reset_ret = 2;
    if (CEngine_OnPostDeviceReset(&e) != false) { printf("FAIL ret2\n"); return 1; }
    // base pointer must be this-0xc
    if ((char*)g_last_base != (char*)&e - 0xc) { printf("FAIL base ptr\n"); return 1; }

    // Case 3: flag set, reset returns 0 -> true
    e.flag_b1 = 1;
    g_reset_ret = 0;
    if (CEngine_OnPostDeviceReset(&e) != true) { printf("FAIL ret0\n"); return 1; }

    // Case 4: flag set, reset returns 5 -> true
    e.flag_b1 = 1;
    g_reset_ret = 5;
    if (CEngine_OnPostDeviceReset(&e) != true) { printf("FAIL ret5\n"); return 1; }

    printf("BEHAVIOUR_OK\n");
    return 0;
}