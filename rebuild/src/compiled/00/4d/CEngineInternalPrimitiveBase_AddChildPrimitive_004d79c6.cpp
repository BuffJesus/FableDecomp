__declspec(naked) bool AddChildPrimitive(){
  __asm {
    push ebp
    mov ebp,esp
    push ecx
    push edi
    xor eax,eax
    lea edi,[ebp-1]
    stosb
    mov al,[ebp-1]
    pop edi
    leave
    ret
  }
}