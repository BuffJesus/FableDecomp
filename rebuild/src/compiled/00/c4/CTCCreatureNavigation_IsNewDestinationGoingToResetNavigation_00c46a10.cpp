__declspec(naked) bool __fastcall IsNewDestinationGoingToResetNavigation(void* self, void* dest)
{
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