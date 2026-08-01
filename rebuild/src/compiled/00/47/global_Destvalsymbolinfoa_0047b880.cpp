#pragma optimize("s",on)
// _Dest_val<std::allocator<std::pair<CCharString,CSymbolMap::CSymbolInfo> >,
//           std::pair<CCharString,CSymbolMap::CSymbolInfo> >  @ 0x0047b880
//
// The manifest slice (87 bytes) OVER-CAPTURES: the real _Dest_val is only the
// leading 27 bytes (0x00..0x1a).  A `jmp` (tail-call) at 0x16 ends it, then five
// int3 fillers (0x1b..0x1f) and a WHOLE ADJACENT allocate+construct function
// (0x20..0x56, ends `ret 0xc`).  Only the leading real function is reconstructed
// here; real_len = 0x1b (27).
//
// Retail disasm of the leading function:
//   push esi
//   mov  esi,ecx          ; esi = this  (saved across the __fastcall dtor calls)
//   lea  ecx,[esi+8]      ; &member @ +0x8
//   call ~M               ; destroy member @ +0x8
//   lea  ecx,[esi+4]      ; &member @ +0x4
//   call ~M               ; destroy member @ +0x4   (SAME dtor target)
//   mov  ecx,esi          ; this
//   pop  esi
//   jmp  ~M               ; tail-call: destroy base object @ +0x0 (SAME target)
//
// The destroyed pair<CCharString,CSymbolInfo> holds three sub-objects that share
// one destructor entry point (the CCharString-style release @ 0x51fc90): one at
// +0x8, one at +0x4, and the base at +0x0.  Because all three go to the same
// symbol, they are declared as a single __fastcall dtor called three times; the
// last call is on the whole object (+0x0) and is the last statement, so cl /O2
// tail-optimises it into `mov ecx,esi ; pop esi ; jmp`.  The rel32 call/jmp
// targets are relocation-masked in parity, so the concrete dtor address is
// immaterial -- only the opcodes and the esi-holds-this shape matter.
//
// Mirrors the already-landed genuine-C++ sibling _Dest_val @ 0x004047a0
// (Destvalstd_allocatorCMacroCMacro), the same push-esi / lea-ecx / call* /
// tail-jmp destroy idiom.

struct Member { char pad[4]; };   // one dword sub-object

// Single relocation-masked destructor entry (this in ecx, __fastcall shape).
extern "C" void __fastcall Symbol_dtor(void* self_ecx);

struct Host {
    char    pad0[4];   // 0x00 .. 0x03  (base object)
    Member  m_4;       // 0x04
    Member  m_8;       // 0x08
};

void __fastcall Dest_val_symbolinfo(Host* self)
{
    Symbol_dtor(&self->m_8);   // destroy member @ +0x8
    Symbol_dtor(&self->m_4);   // destroy member @ +0x4
    Symbol_dtor(self);         // tail-call: destroy base object @ +0x0
}