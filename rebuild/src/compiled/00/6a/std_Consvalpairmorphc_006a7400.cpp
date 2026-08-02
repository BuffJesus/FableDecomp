
// _Cons_val forwarding thunk: MSVC folded this pair-construction helper
// into a tail call to an identical-shape helper (linker/codegen ICF-style
// fold of two _Cons_val<...> instantiations with the same layout as the
// sibling at 006a7140).
// Bytes: mov ecx,[esp+4] ; call target ; ret 4
//
// The thunk itself is __stdcall taking one pointer argument (so the
// compiler loads that argument from the stack and cleans up 4 bytes on
// return), and it forwards that same pointer as the ecx ("this") argument
// of the real callee, which is __fastcall.

extern "C" void __fastcall Real_Cons_val_pair_morph_c(void* self);

extern "C" void __stdcall Cons_val_pair_morph_c(void* ptr)
{
    Real_Cons_val_pair_morph_c(ptr);
}