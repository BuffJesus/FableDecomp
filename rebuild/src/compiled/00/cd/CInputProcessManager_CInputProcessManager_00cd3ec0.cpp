// CInputProcessManager destructor at 0x00cd3ec0 (VC7.1)
// retail:
//   push esi; mov esi,ecx; mov [esi],0x012c2648; call 0xcbeeb0;
//   mov ecx,esi; pop esi; jmp 0x9b02e0

extern "C" void member_dtor_cbeeb0(void);  // call target 0xcbeeb0
extern "C" void base_dtor_9b02e0(void);     // jmp target 0x9b02e0

extern const unsigned int IPM_vtable_012c2648;

__declspec(naked) void DtorIPM_H(void* /*ecx=this*/)
{
    __asm {
        push esi
        mov  esi, ecx
        mov  dword ptr [esi], offset IPM_vtable_012c2648
        call member_dtor_cbeeb0
        mov  ecx, esi
        pop  esi
        jmp  base_dtor_9b02e0
    }
}