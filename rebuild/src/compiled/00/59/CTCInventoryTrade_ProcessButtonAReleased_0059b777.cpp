// CTCInventoryTrade::ProcessButtonAReleased @ 0x0059b777
// __fastcall (this=ecx). When field +0x1c == 1, dispatch vtable slot [0x110];
// then unconditionally tail-jmp to the button-A helper with this preserved.
// Trailing two jmps are adjacent thunks captured in the extracted slice.
extern "C" void ProcessButtonAReleased_tail(void);   // 0x005bc65c
extern "C" void ProcessButtonAReleased_thunk1(void); // 0x005bdd8c
extern "C" void ProcessButtonAReleased_thunk2(void); // 0x005bddb8

__declspec(naked) void ProcessButtonAReleased()
{
    __asm {
        push esi
        mov  esi, ecx
        cmp  dword ptr [esi+0x1c], 1
        jne  skip
        mov  eax, dword ptr [esi]
        call dword ptr [eax+0x110]
    skip:
        mov  ecx, esi
        pop  esi
        jmp  ProcessButtonAReleased_tail
        jmp  ProcessButtonAReleased_thunk1
        jmp  ProcessButtonAReleased_thunk2
    }
}