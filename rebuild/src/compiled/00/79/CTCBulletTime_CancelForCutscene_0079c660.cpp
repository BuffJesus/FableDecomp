extern void helper_0x4c9840(void);
__declspec(naked) void CancelForCutscene(void) {
  __asm {
    mov byte ptr [ecx+0x56], 1
    mov byte ptr [ecx+0x54], 0
    mov ecx, dword ptr [ecx+4]
    push 0x92
    call helper_0x4c9840
    ret
  }
}