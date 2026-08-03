#include <stdio.h>

struct V4 { int a, b, c, d; };
struct GFScreen { int a, b, c, d; };

// Function under test (thiscall: this=ecx, one stack arg, ret 4, returns this in eax)
__declspec(naked) void GFToScreen()
{
    __asm {
        mov eax, ecx
        mov ecx, dword ptr [esp+4]
        mov edx, dword ptr [ecx]
        mov dword ptr [eax], edx
        mov edx, dword ptr [ecx+4]
        mov dword ptr [eax+4], edx
        mov edx, dword ptr [ecx+8]
        mov dword ptr [eax+8], edx
        mov ecx, dword ptr [ecx+0Ch]
        mov dword ptr [eax+0Ch], ecx
        ret 4
    }
}

int main()
{
    V4 src = { 0x11111111, 0x22222222, 0x33333333, 0x44444444 };
    GFScreen dst = { 0, 0, 0, 0 };
    V4* psrc = &src;
    GFScreen* pdst = &dst;
    GFScreen* ret = 0;

    __asm {
        mov ecx, pdst      // this
        push psrc          // stack arg
        call GFToScreen    // callee cleans up (ret 4)
        mov ret, eax
    }

    if (ret != pdst) { printf("FAIL ret\n"); return 1; }
    if (dst.a != 0x11111111 || dst.b != 0x22222222 ||
        dst.c != 0x33333333 || dst.d != 0x44444444) {
        printf("FAIL fields %08x %08x %08x %08x\n", dst.a, dst.b, dst.c, dst.d);
        return 1;
    }
    printf("OK_0x0041a7c0\n");
    return 0;
}