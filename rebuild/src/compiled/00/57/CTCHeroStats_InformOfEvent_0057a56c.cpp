// InformOfEvent @ 0x0057a56c  (CTCHeroStats)
// Forwarding thunk: obj = [esp+4]; forward incoming ecx as arg; call obj->vtbl[0](obj, ecx); ret 4.
__declspec(naked) void InformOfEvent()
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [eax]
        push ecx
        mov ecx, eax
        call dword ptr [edx]
        ret 4
    }
}