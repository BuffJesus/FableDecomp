// CLandscapeBackgroundPatch::`vector_deleting_destructor'
// Retail 0x0048b4c0, __fastcall(this, uint flags) -> void*
// Standard MSVC vector-deleting-destructor: call the scalar destructor,
// then if (flags & 1) operator delete(this), return this.

struct CLandscapeBackgroundPatch;

// The scalar destructor (retail 0x00a0d3d0) — external, produces a real `call`.
void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self);

// operator delete (retail 0x00bfe9bc) — __cdecl free.
void operator_delete_impl(void* p);

struct CLandscapeBackgroundPatch
{
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(this);
    if (flags & 1)
        operator_delete_impl(this);
    return this;
}