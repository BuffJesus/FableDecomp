#include <stdio.h>

struct Arg2 { int f0, f4, f8, fc, f10; };

// Function-under-test: exact naked transcription of retail 0x0049c7a0
// __fastcall ctor: this in ecx, (int a1, Arg2* a2) on stack, ret 8.
__declspec(naked) void CThingSearchTools()
{
    __asm {
        mov edx, dword ptr [esp+4]
        mov eax, ecx
        mov ecx, dword ptr [esp+8]
        mov dword ptr [eax+8], edx
        mov dword ptr [eax], ecx
        xor edx, edx
        mov dword ptr [eax+0Ch], edx
        mov dword ptr [eax+10h], edx
        mov dword ptr [eax+14h], edx
        mov ecx, dword ptr [ecx+10h]
        mov dword ptr [eax+4], ecx
        ret 8
    }
}

int main()
{
    Arg2 a2;
    a2.f0 = 111; a2.f4 = 222; a2.f8 = 333; a2.fc = 444; a2.f10 = 0xABCD;

    // 6-int object, pre-seeded with a sentinel to prove every field is written.
    int obj[6];
    for (int i = 0; i < 6; ++i) obj[i] = 0x55555555;

    void*  pobj = obj;
    int    a1   = 0x1234;
    Arg2*  pa2  = &a2;
    void (*fn)() = CThingSearchTools;

    // Invoke __fastcall: ecx=this, push args right-to-left, callee cleans (ret 8).
    __asm {
        mov  ecx, pobj
        push pa2
        push a1
        call fn
    }

    if (obj[2] != 0x1234)     { printf("bad f8=%d\n",  obj[2]); return 1; }   // +0x8
    if (obj[0] != (int)&a2)   { printf("bad f0\n");             return 1; }   // +0x0
    if (obj[3] != 0)          { printf("bad fc=%d\n",  obj[3]); return 1; }   // +0xc
    if (obj[4] != 0)          { printf("bad f10=%d\n", obj[4]); return 1; }   // +0x10
    if (obj[5] != 0)          { printf("bad f14=%d\n", obj[5]); return 1; }   // +0x14
    if (obj[1] != 0xABCD)     { printf("bad f4=%d\n",  obj[1]); return 1; }   // +0x4

    printf("OK_0x0049c7a0 f8=%d f4=%x\n", obj[2], obj[1]);
    return 0;
}