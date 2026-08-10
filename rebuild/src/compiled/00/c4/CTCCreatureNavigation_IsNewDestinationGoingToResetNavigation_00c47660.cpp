extern "C" __declspec(naked) unsigned char __fastcall IsNewDestinationGoingToResetNavigation(void* self, int edx, void* v, float f) {
    __asm {
        push esi
        mov esi, dword ptr [esp+8]
        mov eax, ecx
        mov ecx, dword ptr [eax+4]
        mov edx, dword ptr [ecx]
        push esi
        push eax
        call dword ptr [edx+0x18]
        pop esi
        ret 4
    }
}