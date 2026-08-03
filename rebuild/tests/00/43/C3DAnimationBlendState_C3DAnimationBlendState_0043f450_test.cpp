#include <stdio.h>
struct Ref { void* pad0; int refcount; };
struct BlendState { void* vtbl; Ref* ref; };
void* const C3DAnimationBlendState_vtable = 0;
struct BlendState* __fastcall C3DAnimationBlendState(struct BlendState* self, int edx_ignored, struct BlendState* other)
{
    self->vtbl = (void*)&C3DAnimationBlendState_vtable;
    Ref* r = other->ref;
    self->ref = r;
    if (r)
        r->refcount++;
    return self;
}
int main()
{
    Ref shared; shared.pad0 = 0; shared.refcount = 5;
    BlendState src; src.vtbl = 0; src.ref = &shared;
    BlendState dst; dst.vtbl = (void*)0xDEAD; dst.ref = (Ref*)0xBEEF;
    BlendState* ret;
    BlendState* pdst = &dst; BlendState* psrc = &src;
    __asm {
        mov  ecx, pdst
        push psrc
        call C3DAnimationBlendState
        mov  ret, eax
    }
    if (ret != &dst) { printf("bad ret\n"); return 1; }
    if (dst.ref != &shared) { printf("bad ref copy\n"); return 1; }
    if (dst.vtbl != (void*)&C3DAnimationBlendState_vtable) { printf("bad vtbl\n"); return 1; }
    if (shared.refcount != 6) { printf("bad refcount %d\n", shared.refcount); return 1; }

    // null-ref branch: no crash, no increment
    BlendState src2; src2.ref = 0;
    BlendState dst2; dst2.vtbl = 0; dst2.ref = (Ref*)0x1234;
    BlendState* pdst2 = &dst2; BlendState* psrc2 = &src2;
    __asm {
        mov  ecx, pdst2
        push psrc2
        call C3DAnimationBlendState
    }
    if (dst2.ref != 0) { printf("bad null copy\n"); return 1; }

    printf("OK_0x0043f450 refcount=%d\n", shared.refcount);
    return 0;
}