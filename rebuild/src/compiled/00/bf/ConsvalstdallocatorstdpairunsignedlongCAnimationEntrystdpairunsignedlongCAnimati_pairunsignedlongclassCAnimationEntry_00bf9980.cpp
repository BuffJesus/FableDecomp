extern "C" int g_anim_holder_ptr;
extern "C" void sub_bd9af0();

extern "C" void __declspec(naked) ConsVal_Anim_A()
{
    __asm {
        mov eax, ecx
        mov ecx, dword ptr [g_anim_holder_ptr]
        mov ecx, dword ptr [ecx + 0x6b0]
        push edx
        push eax
        call sub_bd9af0
        ret
    }
}