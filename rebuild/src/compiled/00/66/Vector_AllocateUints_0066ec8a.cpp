extern "C" void* __cdecl
Vector_AllocateUints_0066ec8a_Allocate(unsigned long bytes);

extern "C" __declspec(naked) void* __fastcall
Vector_AllocateUints_0066ec8a(
    void* vector, void*, unsigned long count, void* unused)
{
    __asm
    {
        push esi
        xor eax, eax
        mov esi, ecx
        push edi
        mov edi, [esp + 0Ch]
        cmp edi, eax
        mov [esi], eax
        mov [esi + 4], eax
        mov [esi + 8], eax
        je store
        mov eax, edi
        shl eax, 2
        push eax
        call Vector_AllocateUints_0066ec8a_Allocate
        pop ecx

    store:
        mov [esi], eax
        mov [esi + 4], eax
        lea eax, [eax + edi * 4]
        mov [esi + 8], eax
        pop edi
        mov eax, esi
        pop esi
        ret 8
    }
}
