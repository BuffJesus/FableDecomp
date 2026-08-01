// CLandscapeBackgroundPatch::`vector deleting destructor' @ 0x00485420
//
// The 72-byte manifest capture over-captures: it holds the leading real
// function F1 (offsets 0x00-0x1d, a 30-byte `vector deleting destructor'),
// then two int3 alignment fillers (0x1e-0x1f), then four unrelated single-line
// accessor thunks F2..F5. We reconstruct ONLY the leading function F1; the
// gate's real_len (30) trims the oracle slice to F1's ret.
//
// F1 disassembly:
//   push esi; mov esi,ecx           ; esi = this
//   call <scalar dtor>              ; __fastcall dtor(this)  (this already in ecx)
//   test byte ptr [esp+8],1         ; flags & 1
//   je  L; push esi; call <delete>; add esp,4   ; __cdecl operator delete(this)
// L: mov eax,esi; pop esi; ret 4    ; return this
//
// This is the textbook MSVC `vector deleting destructor' thunk: run the scalar
// destructor, then free the storage iff bit 0 of the flags word is set, then
// return the object pointer. __fastcall puts `this` in ecx; we model it as
// __fastcall(self, edx_dummy, flags) so `flags` lands on the stack (read at
// [esp+8] after the push esi) exactly like retail. Both call targets are
// relocation-masked in parity, so their concrete addresses do not matter.
//
// Byte-proven: sibling 0x004133a0 (same module, same name) is the identical
// 30-byte body and lands RELOCATION_MATCH with relocations at 0x4;0x11 (the two
// call rel32s). This is the same spelling.

struct CLandscapeBackgroundPatch;

// __fastcall scalar destructor: `this` arrives in ecx, so declare it __fastcall
// with the object as the leading (ecx) argument -> `call` with no push.
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);

// engine operator delete: __cdecl -> `push esi; call; add esp,4`.
extern "C" void operator_delete_stub(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int /*edx_pad*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete_stub((void*)self);
    }
    return (void*)self;
}