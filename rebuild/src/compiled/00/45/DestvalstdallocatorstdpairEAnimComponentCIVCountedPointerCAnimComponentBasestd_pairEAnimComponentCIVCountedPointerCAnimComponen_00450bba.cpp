extern "C" void __declspec(naked) __fastcall Dest_val()
{
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esi+0x0c]
        test ecx, ecx
        je   L1
        dec  dword ptr [ecx+0x04]
        jne  L2
        mov  eax, dword ptr [ecx]
        call dword ptr [eax+0x04]
    L2:
        and  dword ptr [esi+0x0c], 0
    L1:
        pop  esi
        ret
    }
}