#include <stdio.h>

struct CDateAndTime {
    int a;
    int b;
    int c;
    int d;
    int e;
    char f;
};

__declspec(naked) void CDateAndTime_ctor()
{
    __asm {
        push ebp
        mov  ebp, esp
        mov  eax, ecx
        mov  ecx, dword ptr [ebp+0x08]
        mov  dword ptr [eax], ecx
        mov  ecx, dword ptr [ebp+0x0C]
        mov  dword ptr [eax+0x04], ecx
        mov  ecx, dword ptr [ebp+0x10]
        mov  dword ptr [eax+0x08], ecx
        mov  ecx, dword ptr [ebp+0x14]
        mov  dword ptr [eax+0x0C], ecx
        mov  ecx, dword ptr [ebp+0x18]
        mov  dword ptr [eax+0x10], ecx
        mov  cl, byte ptr [ebp+0x1C]
        mov  byte ptr [eax+0x14], cl
        pop  ebp
        ret  0x18
    }
}

int main()
{
    CDateAndTime obj;
    obj.a = obj.b = obj.c = obj.d = obj.e = 0;
    obj.f = 0;

    void* pobj = &obj;
    int a = 2026, b = 7, c = 30, d = 13, e = 45;
    int fbyte = 0x5A;  /* pushed as a dword; ctor reads low byte -> 'Z' */

    /* __fastcall shim: this=ecx, args pushed right-to-left, callee cleans (ret 0x18) */
    __asm {
        push fbyte
        push e
        push d
        push c
        push b
        push a
        mov  ecx, pobj
        call CDateAndTime_ctor
    }

    int ok = (obj.a == 2026) && (obj.b == 7) && (obj.c == 30) &&
             (obj.d == 13) && (obj.e == 45) && (obj.f == (char)0x5A);

    if (ok) {
        printf("OK_0x0057a376 a=%d b=%d c=%d d=%d e=%d f=%d\n",
               obj.a, obj.b, obj.c, obj.d, obj.e, (int)obj.f);
        return 0;
    }
    printf("FAIL a=%d b=%d c=%d d=%d e=%d f=%d\n",
           obj.a, obj.b, obj.c, obj.d, obj.e, (int)obj.f);
    return 1;
}