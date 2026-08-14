#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x004e3625, __fastcall(self, uint flags) -> void*
//
// Classic MSVC vector-deleting-destructor pattern. The object has a subobject
// at +4 whose (scalar) destructor lives at 0x451ac6 and is called __fastcall
// with ecx = self+4. Then if (flags & 1) operator delete(self).

typedef unsigned int uint;

struct CLandscapeBackgroundPatch;

// Subobject destructor at 0x451ac6, __fastcall (this in ecx, no args).
// Model this-in-ecx as a __fastcall free fn with a single leading pointer arg.
extern void __fastcall sub_dtor_451ac6(void* subobj);

// operator delete (__cdecl), void(void*)
extern void __cdecl fable_operator_delete(void* p);

// Real convention is __fastcall(this in ecx, flags on stack). The harness rewrites
// __fastcall->__fastcall, so to keep `flags` on the STACK (not edx) we add an unused
// edx placeholder as the 2nd __fastcall arg and make `flags` the 3rd (stack) arg.
// This reproduces retail's `test byte ptr [esp+8],1` + `ret 4`.
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx_unused*/, uint flags)
{
    // ecx = self + 4  -> subobject destructor
    sub_dtor_451ac6((char*)self + 4);
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}