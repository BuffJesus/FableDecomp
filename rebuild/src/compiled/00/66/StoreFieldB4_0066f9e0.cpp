extern "C" __declspec(naked) void __fastcall
StoreFieldB4_0066f9e0(
    void* object, void*, unsigned long value)
{
    __asm
    {
        mov eax, [esp + 4]
        mov [ecx + 0b4h], eax
        ret 4
    }
}
