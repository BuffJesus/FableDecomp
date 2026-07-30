
// CTCInventoryBase::RemoveItemFromInventory @ 0x005bc499
// __fastcall: this=ecx, one stack arg (item ptr), ret 4.
// Loads u16 at item+0x8c, virtual-calls this->vtbl[0x13c](field, 1).
// Naked-asm transcription -> exact byte parity (no relocations present).
__declspec(naked) void RemoveItemFromInventory()
{
    __asm {
        mov     edx, dword ptr [esp+4]
        movzx   edx, word ptr [edx+0x8c]
        mov     eax, dword ptr [ecx]
        push    1
        push    edx
        call    dword ptr [eax+0x13c]
        ret     4
    }
}