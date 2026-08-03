#include <stdio.h>

// Function under test: transcribed from retail 0x00419220 (__fastcall/__fastcall, this=ecx).
__declspec(naked) void CGameEventPackage()
{
    __asm {
        mov     eax, ecx
        push    esi
        mov     dword ptr [eax+4], 0
        lea     edx, [eax+0x2e]
        mov     esi, 0x28
        or      cl, 0xff
    loop_top:
        mov     byte ptr [edx-0x22], cl
        mov     byte ptr [edx], 0
        add     edx, 0x28
        dec     esi
        jne     loop_top
        pop     esi
        ret
    }
}

int main()
{
    // Buffer large enough for the whole object: array ends at 0x0c + 40*0x28 = 0x64c.
    static unsigned char buf[0x700];
    for (int i = 0; i < 0x700; ++i) buf[i] = 0x55;  // poison
    void* self = (void*)buf;

    void* ret;
    __asm {
        mov  ecx, self
        call CGameEventPackage
        mov  ret, eax
    }

    int ok = 1;
    // ctor returns this in eax
    if (ret != self) { printf("FAIL: return not this\n"); ok = 0; }
    // field at +4 cleared to 0
    unsigned int f4 = *(unsigned int*)(buf + 4);
    if (f4 != 0) { printf("FAIL: f4=%08x\n", f4); ok = 0; }
    // 40 elements, stride 0x28, byte0=0xff at (0x0c + i*0x28), byte at (0x2e + i*0x28)=0
    for (int i = 0; i < 40; ++i) {
        unsigned char a = buf[0x0c + i * 0x28];
        unsigned char b = buf[0x2e + i * 0x28];
        if (a != 0xff) { printf("FAIL: elem %d a=%02x\n", i, a); ok = 0; break; }
        if (b != 0x00) { printf("FAIL: elem %d b=%02x\n", i, b); ok = 0; break; }
    }
    // untouched byte just past the array should still be poison
    if (buf[0x64c] != 0x55) { printf("FAIL: overran array\n"); ok = 0; }

    if (ok) { printf("OK_0x00419220\n"); return 0; }
    return 1;
}