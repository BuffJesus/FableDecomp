#include <stdio.h>
#include <string.h>

struct CNavigationLayer {
    int   f00;
    int   f04;
    int   f08;
    char  pad0c;
    char  f0d;
    char  pad0e;
    char  pad0f;
    int   f10;
    int   f14;
    int   f18;
    char  pad1c;
    char  f1d;
};

CNavigationLayer* __fastcall CNavigationLayer_ctor(CNavigationLayer* thisp) {
    thisp->f00 = 0;
    thisp->f04 = 0;
    thisp->f08 = 0;
    thisp->f0d = 0;
    thisp->f10 = 0;
    thisp->f14 = 0;
    thisp->f18 = 0;
    thisp->f1d = 0;
    return thisp;
}

int main() {
    CNavigationLayer obj;
    memset(&obj, 0xAB, sizeof(obj));
    CNavigationLayer* ret;
    CNavigationLayer* p = &obj;
    __asm {
        mov ecx, p
        call CNavigationLayer_ctor
        mov ret, eax
    }
    if (ret != &obj) { printf("FAIL ret\n"); return 1; }
    if (obj.f00 != 0) { printf("FAIL f00\n"); return 1; }
    if (obj.f04 != 0) { printf("FAIL f04\n"); return 1; }
    if (obj.f08 != 0) { printf("FAIL f08\n"); return 1; }
    if (obj.f0d != 0) { printf("FAIL f0d\n"); return 1; }
    if (obj.f10 != 0) { printf("FAIL f10\n"); return 1; }
    if (obj.f14 != 0) { printf("FAIL f14\n"); return 1; }
    if (obj.f18 != 0) { printf("FAIL f18\n"); return 1; }
    if (obj.f1d != 0) { printf("FAIL f1d\n"); return 1; }
    if ((unsigned char)obj.pad0c != 0xAB) { printf("FAIL pad0c changed\n"); return 1; }
    printf("OK_0x0047040e\n");
    return 0;
}