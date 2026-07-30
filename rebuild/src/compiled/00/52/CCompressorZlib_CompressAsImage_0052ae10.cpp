
// CCompressorZlib::CompressAsImage @ 0x0052ae10
// Forwarder: __fastcall(this=ecx); vtable slot 5 = [ [ecx]+0x14 ].
// Re-pushes args (arg8, arg4, arg3, arg2, arg1) then virtual-calls, ret 0x20.
__declspec(naked) void CompressAsImage()
{
    __asm {
        mov edx, dword ptr [esp+0x20]
        mov eax, dword ptr [ecx]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        mov edx, dword ptr [esp+0x14]
        push edx
        call dword ptr [eax+0x14]
        ret 0x20
    }
}