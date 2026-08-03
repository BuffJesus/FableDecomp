#include <stdio.h>

struct SDimensions { int a; int b; int c; int d; };

struct CWorldMap {
    char pad[0x58];
    int f58; int f5c; int f60; int f64;
};

__declspec(naked) void __fastcall Impl(void* /*this ecx*/)
{
    __asm {
        mov edx, dword ptr [ecx+0x58]
        mov eax, dword ptr [esp+0x4]
        mov dword ptr [eax], edx
        mov edx, dword ptr [ecx+0x5c]
        mov dword ptr [eax+0x4], edx
        mov edx, dword ptr [ecx+0x60]
        mov dword ptr [eax+0x8], edx
        mov ecx, dword ptr [ecx+0x64]
        mov dword ptr [eax+0xc], ecx
        ret 4
    }
}

int main()
{
    CWorldMap obj;
    obj.f58 = 11; obj.f5c = 22; obj.f60 = 33; obj.f64 = 44;
    SDimensions out; out.a = out.b = out.c = out.d = -1;
    void* pobj = &obj; SDimensions* pout = &out;
    __asm {
        mov eax, pout
        push eax
        mov ecx, pobj
        call Impl
    }
    if (out.a == 11 && out.b == 22 && out.c == 33 && out.d == 44) {
        printf("OK_0x004fb2b0 dims=%d,%d,%d,%d\n", out.a, out.b, out.c, out.d);
        return 0;
    }
    printf("BAD %d,%d,%d,%d\n", out.a, out.b, out.c, out.d);
    return 1;
}