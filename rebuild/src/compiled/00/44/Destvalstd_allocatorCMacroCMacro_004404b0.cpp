// _Dest_val<std::allocator<CMacro>,CMacro> @ 0x004404b0 (leading function only).
//
// Genuine C++ reconstruction (real statements only, no inline assembly).
//
// The retail slice at 0x004404b0 holds ONE real function of 0x23 (35) bytes,
// followed by int3 padding and several unrelated orphan functions; only the
// leading function is reconstructed here (real_len = 35).
//
// The leading function is the unrolled destroy of four adjacent, identically
// typed sub-objects of a host aggregate, each destroyed by the SAME single
// element destructor at 0x0055b060 (this in ecx => __fastcall). MSVC (VC7.1)
// destroys aggregate members in REVERSE declaration order, so the four calls
// come out on +0xc, +0x8, +0x4, +0x0. The host pointer is parked in a
// callee-saved register (esi) across the first three calls, and the final
// destroy on the first member (offset 0) is emitted as a tail-call
// (mov ecx,esi; pop esi; jmp 0x0055b060), because it is the last statement
// and its argument is exactly `self`.
//
// Spelling that reproduces the retail bytes:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0xc]; call 0x55b060
//   lea ecx,[esi+0x8]; call 0x55b060
//   lea ecx,[esi+0x4]; call 0x55b060
//   mov ecx,esi; pop esi; jmp 0x55b060
// A struct with four CMacro members whose destructor is 0x0055b060, invoked
// via a compiler-generated aggregate destructor, yields exactly this. To keep
// all four calls resolving to the SAME rel32 target we route every element
// destroy through one extern __fastcall(this) shim (call/jmp rel32 are
// relocation-masked in parity).

struct CMacro { char pad[4]; };   // 4-byte element (stride matches +0x4 spacing)

// Single element destructor at 0x0055b060, __fastcall (this in ecx).
extern "C" void __fastcall CMacro_dtor(CMacro* self);

struct Host {
    CMacro m_0;   // +0x00  (destroyed last, as tail-call)
    CMacro m_4;   // +0x04
    CMacro m_8;   // +0x08
    CMacro m_c;   // +0x0c  (destroyed first)
};

// _Dest_val destroys the four sub-objects in reverse: +0xc, +0x8, +0x4, +0x0.
void __fastcall Dest_val(Host* self)
{
    CMacro_dtor(&self->m_c);
    CMacro_dtor(&self->m_8);
    CMacro_dtor(&self->m_4);
    CMacro_dtor(&self->m_0);
}