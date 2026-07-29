extern "C" void __fastcall
CManager_GetAllRegisteredScreens_0066e941_Clear(
    void* output, void*, void* begin, void* end);
extern "C" void __fastcall
CManager_GetAllRegisteredScreens_0066e941_Insert(
    void* output, void*, void* value);

extern "C" __declspec(naked) void __fastcall
CManager_GetAllRegisteredScreens_0066e941(void* manager, void*, void* output)
{
    __asm
    {
        push ebx
        mov ebx, [esp + 8]
        push ebp
        push esi
        push edi
        push dword ptr [ebx + 4]
        mov edi, ecx
        push dword ptr [ebx]
        mov ecx, ebx
        call CManager_GetAllRegisteredScreens_0066e941_Clear
        mov esi, [edi + 8]
        sub esi, [edi + 4]
        xor ebp, ebp
        sar esi, 2
        test esi, esi
        jle done

    scan:
        mov eax, [edi + 4]
        lea eax, [eax + ebp * 4]
        cmp dword ptr [eax], 0
        je next
        push eax
        mov ecx, ebx
        call CManager_GetAllRegisteredScreens_0066e941_Insert

    next:
        inc ebp
        cmp ebp, esi
        jl scan

    done:
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret 4
    }
}
