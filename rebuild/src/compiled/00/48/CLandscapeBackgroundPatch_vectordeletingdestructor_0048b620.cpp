// CLandscapeBackgroundPatch::`vector deleting destructor'
// Retail 0x0048b620 — MSVC VC7.1 vector deleting destructor.
//
// push esi; mov esi,ecx; call scalar_dtor; test [esp+8],1;
// je +; push esi; call operator_delete; add esp,4; +: mov eax,esi; pop esi; ret 4

// scalar destructor at 0x00a0d3d0 (this in ecx)
void __fastcall CLandscapeBackgroundPatch_scalar_dtor(void *self);
// operator delete at 0x00bfe9bc (cdecl)
void __cdecl CLandscapeBackgroundPatch_op_delete(void *p);

struct CLandscapeBackgroundPatch
{
    void * __fastcall vector_deleting_destructor(int edx, unsigned int flags);
};

void * __fastcall CLandscapeBackgroundPatch::vector_deleting_destructor(int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_scalar_dtor(this);
    if (flags & 1)
        CLandscapeBackgroundPatch_op_delete(this);
    return this;
}