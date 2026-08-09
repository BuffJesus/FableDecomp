__declspec(naked) bool IsNewDestinationGoingToResetNavigation(void* self, void* dest, float f)
{
    (void)self; (void)dest; (void)f;
    __asm {
        push esi
        mov esi, dword ptr [esp+8]
        mov eax, ecx
        mov ecx, dword ptr [eax+4]
        mov edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x1c]
        pop esi
        ret 4
    }
}