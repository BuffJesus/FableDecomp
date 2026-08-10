extern "C" void __fastcall CharCtor(void* self, int edx, const char* s, int n);

__declspec(naked) void* __fastcall GetActionName_6(void* self)
{
    __asm {
        push esi
        push -1
        push 0x123d8b0
        mov esi, ecx
        call CharCtor
        mov eax, esi
        pop esi
        ret
    }
}