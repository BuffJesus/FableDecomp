// CScriptThing::SetFriendsWithEverythingFlag @ 0x004ab060
// Forwarding thunk: load delegate at this+4; if non-null tail-call its
// virtual method at vtable+0x10c (slot 67) passing the same arg; ret 4.
__declspec(naked) void SetFriendsWithEverythingFlag(bool /*flag*/)
{
    __asm
    {
        mov ecx, dword ptr [ecx+0x4]
        test ecx, ecx
        je   done
        mov  eax, dword ptr [ecx]
        jmp  dword ptr [eax+0x10c]
    done:
        ret 0x4
    }
}