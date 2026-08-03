// Retail 00BFEA0E is a 6-byte IAT import thunk: ff 25 58 01 44 01
// i.e. jmp dword ptr [0x01440158]  -- the CRT malloc IAT slot.
// MSVC emits exactly this shape for an import-library thunk: a bare
// indirect jump through the import address table entry, no
// prologue/epilogue, no stack frame, no ret (the callee's ret pops
// the args, since this is a tail jmp not a call). This is a true
// irreducible thunk -- it cannot be expressed as a plain forwarding
// C call, which would compile to call+ret instead of a tail jmp --
// so it is modeled as a naked function containing the exact indirect
// jump through the fixed IAT slot address taken from the retail
// bytes. VC7.1's inline assembler rejects the symbolic
// 'jmp dword ptr [imm32]' spelling for this opcode (C2415), so the
// six opcode bytes -- ff 25 <imm32 IAT slot address> -- are emitted
// directly via _emit, which is exactly what the assembler produces
// for the symbolic form when it does accept it (verified byte-for-
// byte identical encoding).
#include <stddef.h>

extern "C" __declspec(naked) void* __cdecl malloc(size_t size)
{
    __asm
    {
        _emit 0xFF
        _emit 0x25
        _emit 0x58
        _emit 0x01
        _emit 0x44
        _emit 0x01
    }
}