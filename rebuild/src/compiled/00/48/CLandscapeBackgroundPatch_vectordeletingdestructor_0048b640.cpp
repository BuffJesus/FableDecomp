// CLandscapeBackgroundPatch::`vector_deleting_destructor'  (retail 0x0048b640)
//
// MSVC VC7.1 compiler-generated vector deleting destructor shape:
//   push esi; mov esi,ecx; call <scalar_dtor>(this)
//   test byte ptr [esp+8],1 ; if(flags&1) operator delete(this)
//   return this;
//
// The scalar destructor (retail 0x00a0d3d0) and operator delete
// (retail 0x00bfe9bc) are external calls resolved by relocation.

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
extern "C" void __cdecl operator_delete_LBP(void* p);

// vector deleting destructor: __fastcall (this in ecx, flags on stack).
// Modelled as __fastcall with a dummy edx slot so 'flags' is passed on the
// stack (ecx=self, edx=unused), matching retail's 'test [esp+8],1'.
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_LBP(self);
    return self;
}