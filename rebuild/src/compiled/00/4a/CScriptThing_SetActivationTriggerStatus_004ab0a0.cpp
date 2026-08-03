// CScriptThing::SetActivationTriggerStatus @ 0x004ab0a0
// Forwarder: member object at [this+4]; if non-null tail-calls its virtual
// method at vtable offset 0x114 passing the single stack arg through.
// __fastcall(this=ecx, edx, status-on-stack) reproduces the __fastcall frame:
// one 4-byte stack arg -> "ret 4".
__declspec(naked) void __fastcall SetActivationTriggerStatus(void* ecx, void* edx, int status)
{
    (void)ecx; (void)edx; (void)status;
    __asm {
        mov  ecx, dword ptr [ecx+4]
        test ecx, ecx
        je   short l_done
        mov  eax, dword ptr [ecx]
        jmp  dword ptr [eax+0x114]
    l_done:
        ret  4
    }
}