struct CCharString { void* p; };

extern "C" void __fastcall Helper_99ebf0(CCharString* self, int /*edx*/, const char* s, int n);

__declspec(naked) CCharString* __fastcall GetActionName_4(CCharString* self)
{
    __asm {
        push esi
        push -1
        push 0x123baec
        mov esi, ecx
        call Helper_99ebf0
        mov eax, esi
        pop esi
        ret
    }
}