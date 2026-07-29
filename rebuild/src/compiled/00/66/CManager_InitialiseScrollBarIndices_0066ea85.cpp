extern "C" void __fastcall
CManager_InitialiseScrollBarIndices_0066ea85_Clear(
    void* vector, void*, void* begin, void* end);
extern "C" void __fastcall
CManager_InitialiseScrollBarIndices_0066ea85_Resize(
    void* vector, void*, long count);

extern "C" __declspec(naked) void __fastcall
CManager_InitialiseScrollBarIndices_0066ea85(
    void* manager, void*, long count)
{
    __asm
    {
        push esi
        lea esi, [ecx + 1Ch]
        push edi
        push dword ptr [esi + 4]
        mov ecx, esi
        push dword ptr [esi]
        call CManager_InitialiseScrollBarIndices_0066ea85_Clear
        mov edi, [esp + 0Ch]
        push edi
        mov ecx, esi
        call CManager_InitialiseScrollBarIndices_0066ea85_Resize
        xor eax, eax
        test edi, edi
        jle done

    fill:
        mov ecx, [esi]
        mov [ecx + eax * 4], eax
        inc eax
        cmp eax, edi
        jl fill

    done:
        pop edi
        pop esi
        ret 4
    }
}
