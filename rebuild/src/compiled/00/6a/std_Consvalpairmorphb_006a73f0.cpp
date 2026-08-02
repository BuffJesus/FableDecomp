// Cons_val_pair_morph_b (retail 0x006a73f0)
// Thiscall-forwarding thunk: takes a single pointer argument off the stack
// (as pushed by the caller), moves it into ECX, and tail-calls the real
// pair-value constructor helper at retail 0x0085f0c0 (this-in-ecx), then
// pops the one stack argument (ret 4).
//
// Retail bytes: 8b 4c 24 04   mov ecx,[esp+4]
//               e8 c7 7c 1b 00 call 0x0085f0c0
//               c2 04 00      ret 4

struct MorphPair; // opaque; only address is ever touched by this thunk

// Real constructor helper, __fastcall (this in ECX, no stack args) so the
// call site matches the retail `call` + no extra pushes, and the callee's
// own `ret` (not modeled here) cleans nothing else up.
extern "C" void __fastcall MorphPair_CtorHelper(MorphPair* self);

extern "C" void __stdcall Cons_val_pair_morph_b(MorphPair* _Ptr)
{
    MorphPair_CtorHelper(_Ptr);
}