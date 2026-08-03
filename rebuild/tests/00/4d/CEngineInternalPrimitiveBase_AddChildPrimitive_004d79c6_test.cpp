#include <cstdio>
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
int main(){
  bool r = AddChildPrimitive();
  if (r != false){ printf("BAD r=%d\n",(int)r); return 1; }
  printf("OK_0x004d79c6 r=%d\n",(int)r);
  return 0;
}