#pragma optimize("s",on)
// CLandscapeBackgroundPatch scalar/vector deleting destructor @ 0x004942a2
// __fastcall (self in ecx, uint flags on stack), returns void*, ret 4
// push esi; mov esi,ecx; call inner_dtor; test byte[esp+8],1;
// je S; push esi; call operator delete; pop ecx; S: mov eax,esi; pop esi; ret 4
//
// __fastcall is rewritten to __fastcall by the harness. To keep `flags` on the
// STACK (not edx), we give the fn a dummy edx slot so the real flags arg spills
// to the stack, reproducing `test byte[esp+8],1` and `ret 4`.

struct CLandscapeBackgroundPatch;

extern "C" void __fastcall LBP_dtor(CLandscapeBackgroundPatch* self);   // inner (non-deleting) dtor
extern "C" void __cdecl LBP_op_delete(void* p);                          // operator delete

extern "C" void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    LBP_dtor(self);
    if (flags & 1)
        LBP_op_delete(self);
    return self;
}