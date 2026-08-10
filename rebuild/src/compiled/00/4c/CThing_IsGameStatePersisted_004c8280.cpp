__declspec(naked) bool IsGameStatePersisted(void* self)
{
  __asm {
    mov al, byte ptr [ecx+0x38]
    test al, al
    jns short L1
    xor al, al
    ret
  L1:
    mov al, byte ptr [ecx+0x92]
    shr al, 5
    and al, 1
    ret
  }
}