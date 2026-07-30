#include <stdio.h>

struct GFV16 { long m0, m1, m2, m3; };

struct GFV16* __fastcall GFToScreen(struct GFV16* self, int /*edx*/, const struct GFV16* src)
{
    self->m0 = src->m0;
    self->m1 = src->m1;
    self->m2 = src->m2;
    self->m3 = src->m3;
    return self;
}

int main()
{
    GFV16 dst = { 0, 0, 0, 0 };
    GFV16 src = { 0x11111111, 0x22222222, 0x33333333, 0x44444444 };

    GFV16* ret;
    void* pdst = &dst;
    void* psrc = &src;
    // __fastcall/__fastcall shim: ecx=this, one stack arg = src. Callee does ret 4.
    __asm {
        push psrc
        mov  ecx, pdst
        call GFToScreen
        mov  ret, eax
    }

    if (dst.m0 == 0x11111111 && dst.m1 == 0x22222222 &&
        dst.m2 == 0x33333333 && dst.m3 == 0x44444444 &&
        ret == &dst)
    {
        printf("OK_0x004935f8\n");
        return 0;
    }
    printf("BAD %08lx %08lx %08lx %08lx ret=%p\n", dst.m0, dst.m1, dst.m2, dst.m3, ret);
    return 1;
}