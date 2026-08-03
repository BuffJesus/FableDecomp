// CScriptThing::IncrementScriptCounter @ 0x004ab0f0
// Loads the counter/context object at this+0x4; if non-null, tail-calls its
// virtual method at vtable slot 0x120 (index 72). __fastcall (this=ecx).
//
// Disassembly (16 bytes):
//   8b 49 04        mov  ecx, [ecx+0x4]
//   85 c9           test ecx, ecx
//   74 08           je   +8
//   8b 01           mov  eax, [ecx]
//   ff a0 20 01 00 00 jmp  dword ptr [eax+0x120]
//   c3              ret
__declspec(naked) void IncrementScriptCounter()
{
    __asm {
        mov ecx, dword ptr [ecx+4]
        test ecx, ecx
        je   done
        mov  eax, dword ptr [ecx]
        jmp  dword ptr [eax+0x120]
    done:
        ret
    }
}