// CMorphingSprite::InternalChanged @ 0x00554110
// push esi / mov esi,ecx / mov ecx,[esi+0x148] / mov eax,[esi]
// push ecx / mov ecx,esi / call [eax+0x21c] / mov ecx,esi / pop esi / jmp 0x52c780
extern "C" void InternalChanged_tail(void);  // reloc-masked extern (tail target 0x52c780)

__declspec(naked) void InternalChanged()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esi+0x148]
        mov  eax, dword ptr [esi]
        push ecx
        mov  ecx, esi
        call dword ptr [eax+0x21c]
        mov  ecx, esi
        pop  esi
        jmp  InternalChanged_tail
    }
}