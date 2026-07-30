#include <stdio.h>

struct Vec2 { int x; int y; };

__declspec(naked) void __fastcall GetClosestPointToPos2D(void *thisptr, void *other)
{
    (void)thisptr; (void)other;
    __asm {
        sub  esp, 8
        mov  eax, dword ptr [ecx]
        push esi
        mov  esi, dword ptr [edx]
        mov  dword ptr [ecx], esi
        mov  esi, dword ptr [edx+4]
        mov  dword ptr [esp+4], eax
        mov  eax, dword ptr [ecx+4]
        mov  dword ptr [ecx+4], esi
        mov  ecx, dword ptr [esp+4]
        mov  dword ptr [edx], ecx
        mov  dword ptr [edx+4], eax
        pop  esi
        add  esp, 8
        ret
    }
}

int main()
{
    Vec2 a = { 1, 2 };
    Vec2 b = { 10, 20 };
    Vec2 *pa = &a;
    Vec2 *pb = &b;

    __asm {
        mov ecx, pa
        mov edx, pb
        call GetClosestPointToPos2D
    }

    if (a.x == 10 && a.y == 20 && b.x == 1 && b.y == 2) {
        printf("OK_0x004cf3d0\n");
        return 0;
    }
    printf("FAIL a=(%d,%d) b=(%d,%d)\n", a.x, a.y, b.x, b.y);
    return 1;
}