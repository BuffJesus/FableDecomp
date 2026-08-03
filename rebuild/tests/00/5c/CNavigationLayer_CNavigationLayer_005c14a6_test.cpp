#include <stdio.h>
#include <string.h>

// function-under-test: CNavigationLayer ctor (naked, this=ecx, returns this)
__declspec(naked) void CNavigationLayer()
{
    __asm {
        mov     eax, ecx
        xor     ecx, ecx
        mov     dword ptr [eax], ecx
        mov     dword ptr [eax+0x4], ecx
        mov     dword ptr [eax+0x8], ecx
        or      dword ptr [eax+0x1c], 0xffffffff
        mov     dword ptr [eax+0xc], ecx
        mov     dword ptr [eax+0x10], ecx
        mov     dword ptr [eax+0x14], ecx
        mov     byte ptr [eax+0x18], 1
        ret
    }
}

int main()
{
    unsigned char buf[0x20];
    memset(buf, 0xAA, sizeof(buf));

    void* self = 0;
    void (*fn)() = CNavigationLayer;
    __asm {
        lea     ecx, buf
        call    fn
        mov     self, eax
    }

    if (self != (void*)buf) { printf("FAIL: return this=%p buf=%p\n", self, (void*)buf); return 1; }

    int   f00 = *(int*)(buf + 0x00);
    int   f04 = *(int*)(buf + 0x04);
    int   f08 = *(int*)(buf + 0x08);
    int   f0c = *(int*)(buf + 0x0c);
    int   f10 = *(int*)(buf + 0x10);
    int   f14 = *(int*)(buf + 0x14);
    unsigned char f18 = *(unsigned char*)(buf + 0x18);
    int   f1c = *(int*)(buf + 0x1c);

    if (f00 || f04 || f08 || f0c || f10 || f14) { printf("FAIL: zero fields %d %d %d %d %d %d\n", f00,f04,f08,f0c,f10,f14); return 1; }
    if (f18 != 1)  { printf("FAIL: [0x18]=%d expected 1\n", (int)f18); return 1; }
    if (f1c != -1) { printf("FAIL: [0x1c]=%d expected -1\n", f1c); return 1; }
    if (buf[0x19] != 0xAA) { printf("FAIL: [0x19] clobbered =%02x\n", buf[0x19]); return 1; }

    printf("OK_0x005c14a6 ctor fields verified\n");
    return 0;
}