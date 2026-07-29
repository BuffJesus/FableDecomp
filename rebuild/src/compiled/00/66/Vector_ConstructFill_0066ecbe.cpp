extern "C" void* __fastcall
Vector_AllocateUints_0066ec8a(
    void* vector, void*, unsigned long count, void* unused);
extern "C" void __fastcall
std_Fill_n_ulong_0066ec31(
    unsigned long* destination, unsigned long count, const unsigned long* value);

extern "C" __declspec(naked) void* __fastcall
Vector_ConstructFill_0066ecbe(
    void* vector, void*, unsigned long count,
    const unsigned long* value, void* allocator)
{
    __asm
    {
        push esi
        push dword ptr [esp + 10h]
        mov esi, ecx
        push dword ptr [esp + 0Ch]
        call Vector_AllocateUints_0066ec8a
        push dword ptr [esp + 0Ch]
        mov ecx, [esi]
        mov edx, [esp + 0Ch]
        call std_Fill_n_ulong_0066ec31
        mov [esi + 4], eax
        mov eax, esi
        pop esi
        ret 0Ch
    }
}
