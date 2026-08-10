struct CCharString { void* p; };

extern "C" void Ctor_99ebf0(void);

__declspec(naked) CCharString* __fastcall GetActionName_3(CCharString* self)
{
    __asm {
        push esi
        push -1
        push 0x123ba5c
        mov  esi, ecx
        call Ctor_99ebf0
        mov  eax, esi
        pop  esi
        ret
    }
}