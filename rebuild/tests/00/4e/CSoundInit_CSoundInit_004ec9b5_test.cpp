#include <stdio.h>

__declspec(naked) void __fastcall CSoundInit()
{
    __asm {
        mov     eax, ecx
        fldz
        xor     ecx, ecx
        mov     dword ptr [eax], ecx
        fstp    dword ptr [eax+4]
        mov     dword ptr [eax+0x18], ecx
        mov     dword ptr [eax+0x1c], ecx
        mov     dword ptr [eax+0x20], ecx
        mov     dword ptr [eax+0x24], ecx
        mov     dword ptr [eax+0x28], ecx
        mov     dword ptr [eax+0x2c], ecx
        ret
    }
}

struct CSoundInit_t {
    int   f00;
    float f04;
    int   f08, f0c, f10, f14;
    int   f18, f1c, f20, f24, f28, f2c;
};

int main()
{
    CSoundInit_t s;
    unsigned char* p = (unsigned char*)&s;
    for (int i = 0; i < (int)sizeof(s); ++i) p[i] = 0xCD;

    CSoundInit_t* ret;
    void* self = &s;
    __asm {
        mov  ecx, self
        call CSoundInit
        mov  ret, eax
    }

    if (ret != &s) { printf("FAIL: return value\n"); return 1; }
    if (s.f00 != 0)    { printf("FAIL: f00\n"); return 1; }
    if (s.f04 != 0.0f) { printf("FAIL: f04\n"); return 1; }
    if (s.f18 != 0)    { printf("FAIL: f18\n"); return 1; }
    if (s.f1c != 0)    { printf("FAIL: f1c\n"); return 1; }
    if (s.f20 != 0)    { printf("FAIL: f20\n"); return 1; }
    if (s.f24 != 0)    { printf("FAIL: f24\n"); return 1; }
    if (s.f28 != 0)    { printf("FAIL: f28\n"); return 1; }
    if (s.f2c != 0)    { printf("FAIL: f2c\n"); return 1; }
    if (s.f08 != (int)0xCDCDCDCD) { printf("FAIL: f08 touched\n"); return 1; }
    if (s.f14 != (int)0xCDCDCDCD) { printf("FAIL: f14 touched\n"); return 1; }

    printf("OK_0x004ec9b5\n");
    return 0;
}