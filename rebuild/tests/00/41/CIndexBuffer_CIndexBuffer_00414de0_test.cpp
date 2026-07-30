#include <stdio.h>

struct CIndexBuffer {
    int f0;
    int f4;
    int f8;
};

// __fastcall ctor: this in ecx, one stack arg, ret 4
__declspec(naked) void CIndexBuffer_ctor()
{
    __asm {
        mov eax, ecx
        xor ecx, ecx
        mov dword ptr [eax], ecx
        mov dword ptr [eax+4], ecx
        mov dword ptr [eax+8], ecx
        ret 4
    }
}

int main()
{
    CIndexBuffer obj;
    obj.f0 = 0x11111111;
    obj.f4 = 0x22222222;
    obj.f8 = 0x33333333;
    void* pobj = &obj;
    int arg = 12345;
    void (*fn)() = CIndexBuffer_ctor;
    __asm {
        push arg
        mov ecx, pobj
        call fn
    }
    if (obj.f0 == 0 && obj.f4 == 0 && obj.f8 == 0) {
        printf("OK_0x00414de0\n");
        return 0;
    }
    printf("FAIL f0=%d f4=%d f8=%d\n", obj.f0, obj.f4, obj.f8);
    return 1;
}