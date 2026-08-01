// _Dest_val<std::allocator<std::pair<CCharString,CSymbolMap::CSymbolInfo> >, std::pair<CCharString,CSymbolMap::CSymbolInfo> >  @ 0x0047b880
//
// The manifest length (87 bytes) OVER-CAPTURES: the real _Dest_val is only the
// first 27 bytes (0x00-0x1a). It is followed by 5 int3 alignment fillers
// (0x1b-0x1f) and then a WHOLE ADJACENT function (0x20-0x56, an allocator
// allocate+construct wrapper that ends `ret 0xc`). The parity gate compares the
// full 87-byte oracle slice, and a per-symbol .obj .text disasm cannot reproduce
// the linker-inserted int3 padding between two separately-compiled functions.
// So the honest way to emit exactly these 87 contiguous bytes under ONE symbol
// is one naked function that lays down: routine 1, the 5 int3 fillers, routine 2.
// (Mirrors landed naked siblings Vector_ConstructFill_0066ecbe /
// Uninit_copy_uint_047b670.)
//
// Routine 1 (0x00-0x1a) = the actual _Dest_val: this in ecx (esi); destroy the
//   pair members at +0x8 and +0x4, then tail-jmp the base dtor at +0x0.
// Routine 2 (0x20-0x56) = allocator allocate(count) of count*12-byte pairs then
//   a __fastcall construct call; returns the block, ret 0xc.
// All five rel32 call/jmp targets are relocation-masked in parity, so the exact
// extern targets do not matter.

extern "C" void __fastcall _Dest_val_sub_dtor(void* member_ecx);
extern "C" void __fastcall _Dest_val_base_dtor(void* self_ecx);
extern "C" void* __cdecl _Dest_val_operator_new(unsigned int bytes);
extern "C" void __fastcall _Dest_val_construct(void* dst_ecx, unsigned int n_edx, void* whence_stack);

extern "C" __declspec(naked) void __fastcall _Dest_val_symbolinfo_a(void* /*self in ecx*/)
{
    __asm
    {
        // ---- routine 1: the real _Dest_val (0x00-0x1a) ----
        push esi                          // 0x00: 56
        mov  esi, ecx                     // 0x01: 8b f1
        lea  ecx, [esi + 8]               // 0x03: 8d 4e 08
        call _Dest_val_sub_dtor           // 0x06: e8 rel32
        lea  ecx, [esi + 4]               // 0x0b: 8d 4e 04
        call _Dest_val_sub_dtor           // 0x0e: e8 rel32
        mov  ecx, esi                     // 0x13: 8b ce
        pop  esi                          // 0x15: 5e
        jmp  _Dest_val_base_dtor          // 0x16: e9 rel32 (tail-call)

        // ---- 5 int3 alignment fillers (0x1b-0x1f) ----
        int 3
        int 3
        int 3
        int 3
        int 3

        // ---- routine 2: adjacent allocate+construct wrapper (0x20-0x56) ----
        mov  eax, dword ptr [esp + 4]     // 0x20: 8b 44 24 04
        test eax, eax                     // 0x24: 85 c0
        push esi                          // 0x26: 56
        je   L_zero                       // 0x27: 74 13
        lea  eax, [eax + eax*2]           // 0x29: 8d 04 40
        shl  eax, 2                       // 0x2c: c1 e0 02
        push eax                          // 0x2f: 50
        call _Dest_val_operator_new       // 0x30: e8 rel32
        add  esp, 4                       // 0x35: 83 c4 04
        mov  esi, eax                     // 0x38: 8b f0
        jmp  L_have                       // 0x3a: eb 02
    L_zero:
        xor  esi, esi                     // 0x3c: 33 f6
    L_have:
        mov  edx, dword ptr [esp + 10h]   // 0x3e: 8b 54 24 10
        lea  ecx, [esp + 8]               // 0x42: 8d 4c 24 08
        push ecx                          // 0x46: 51
        mov  ecx, dword ptr [esp + 10h]   // 0x47: 8b 4c 24 10
        push esi                          // 0x4b: 56
        call _Dest_val_construct          // 0x4c: e8 rel32
        mov  eax, esi                     // 0x51: 8b c6
        pop  esi                          // 0x53: 5e
        ret  0Ch                          // 0x54: c2 0c 00
    }
}