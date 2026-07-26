// _Dest_val<std::allocator<CIVCountedPointer<...> >, CIVCountedPointer<...> >
// Counted-pointer release: decrement refcount at [ptr+4]; if it hits 0 call the
// virtual slot at [ [ptr] + 4 ] (thiscall, this in ecx); then null the holder slot.
// The oracle region is 30 bytes and includes a trailing 6-byte adjacent accessor
// (mov eax,[ecx]; add eax,0x10; ret) laid down right after the ret, so this is
// authored as one naked routine to reproduce the exact byte region.
__declspec(naked) void __fastcall _Dest_val(void* self)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, [esi]
        test ecx, ecx
        je   done
        dec  dword ptr [ecx+4]
        jne  clear
        mov  eax, [ecx]
        call dword ptr [eax+4]
    clear:
        and  dword ptr [esi], 0
    done:
        pop  esi
        ret
        // trailing adjacent accessor captured in the same 30-byte region
        mov  eax, [ecx]
        add  eax, 10h
        ret
    }
}