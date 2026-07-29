extern "C" void __fastcall
CManager_GetAllRegisteredInventoryScreens_0066e9e9_Clear(
    void* output, void*, void* begin, void* end);
extern "C" void __fastcall
CManager_GetAllRegisteredInventoryScreens_0066e9e9_Insert(
    void* output, void*, void* value);

extern "C" __declspec(naked) void __fastcall
CManager_GetAllRegisteredInventoryScreens_0066e9e9(
    void* manager, void*, void* output)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        push edi
        mov edi, ecx
        mov ecx, [ebp + 8]
        push dword ptr [ecx + 4]
        push dword ptr [ecx]
        call CManager_GetAllRegisteredInventoryScreens_0066e9e9_Clear
        mov esi, [edi + 8]
        sub esi, [edi + 4]
        and dword ptr [ebp - 4], 0
        sar esi, 2
        test esi, esi
        jle done
        push ebx

    scan:
        mov ebx, [ebp - 4]
        mov eax, [edi + 4]
        shl ebx, 2
        mov ecx, [ebx + eax]
        test ecx, ecx
        je next
        mov eax, [ecx]
        call dword ptr [eax + 0Ch]
        test al, al
        je next
        mov eax, [edi + 4]
        mov ecx, [ebp + 8]
        add eax, ebx
        push eax
        call CManager_GetAllRegisteredInventoryScreens_0066e9e9_Insert

    next:
        inc dword ptr [ebp - 4]
        cmp [ebp - 4], esi
        jl scan
        pop ebx

    done:
        pop edi
        pop esi
        leave
        ret 4
    }
}
