// _Dest_val<std::allocator<CIVCountedPointer<...> >, CIVCountedPointer<...> >
// Counted-pointer release: decrement refcount at [ptr+4]; if it hits 0 call the
// virtual slot at [ [ptr] + 4 ] (thiscall, this in ecx); then null the holder slot.
// Real retail function is 24 bytes; a trailing 6-byte adjacent accessor was
// over-captured and has been dropped.
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
    }
}