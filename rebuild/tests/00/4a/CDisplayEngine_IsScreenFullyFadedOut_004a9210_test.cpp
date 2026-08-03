
#include <stdio.h>

struct CDisplayEngine {
    char pad_bc[0xbc];
    unsigned char fadeActive;   // +0xbc
    char pad_c8[0xc8 - 0xbc - 1];
    unsigned char flagC8;       // +0xc8
    unsigned char flagC9;       // +0xc9
};

int __fastcall IsScreenFullyFadedOut(CDisplayEngine *thisp)
{
    if (thisp->fadeActive) {
        if (!thisp->flagC9) {
            if (!thisp->flagC8) {
                return 1;
            }
        }
    }
    return 0;
}

static int call_it(CDisplayEngine *obj)
{
    int r;
    void *p = obj;
    __asm {
        mov ecx, p
        call IsScreenFullyFadedOut
        mov r, eax
    }
    return r;
}

int main()
{
    CDisplayEngine o;

    // all zero -> fadeActive false -> 0
    o.fadeActive = 0; o.flagC8 = 0; o.flagC9 = 0;
    if (call_it(&o) != 0) { printf("FAIL case1\n"); return 1; }

    // fade active, both flags clear -> 1
    o.fadeActive = 1; o.flagC8 = 0; o.flagC9 = 0;
    if (call_it(&o) != 1) { printf("FAIL case2\n"); return 1; }

    // fade active but flagC9 set -> 0
    o.fadeActive = 1; o.flagC8 = 0; o.flagC9 = 1;
    if (call_it(&o) != 0) { printf("FAIL case3\n"); return 1; }

    // fade active but flagC8 set -> 0
    o.fadeActive = 1; o.flagC8 = 1; o.flagC9 = 0;
    if (call_it(&o) != 0) { printf("FAIL case4\n"); return 1; }

    // fade active, both flags set -> 0
    o.fadeActive = 1; o.flagC8 = 1; o.flagC9 = 1;
    if (call_it(&o) != 0) { printf("FAIL case5\n"); return 1; }

    // nonzero fadeActive value other than 1 -> 1
    o.fadeActive = 0x42; o.flagC8 = 0; o.flagC9 = 0;
    if (call_it(&o) != 1) { printf("FAIL case6\n"); return 1; }

    printf("OK_0x004a9210\n");
    return 0;
}