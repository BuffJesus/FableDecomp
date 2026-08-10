extern "C" void __fastcall sub_a01c10(void* self);
extern "C" void __fastcall sub_693ef0(void* self);

__declspec(naked) void __fastcall DestVal_A8_A(void* a, void* b)
{
    (void)a; (void)b;
    __asm {
        push esi
        mov esi, ecx
        lea ecx, [esi + 0xa8]
        call sub_a01c10
        mov ecx, esi
        pop esi
        jmp sub_693ef0
    }
}

extern "C" __declspec(noinline) void __fastcall sub_a01c10(void* self){ (void)self; }
extern "C" __declspec(noinline) void __fastcall sub_693ef0(void* self){ (void)self; }