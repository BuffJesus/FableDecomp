extern "C" void __cdecl FableFrontEndLeftClick992470FreeControl(void* control);

extern "C" __declspec(naked) void __fastcall
CFrontEndScreen_AddActionOnLeftClicked_00992470(void* screen, void*, void* action)
{
    __asm
    {
        mov eax, [esp + 4]
        push ebx
        mov ebx, [eax]
        push esi
        push edi
        mov edi, [eax + 4]
        mov esi, ecx
        mov eax, [esi + 174h]
        cmp eax, edi
        je done
        test eax, eax
        je assign
        dec dword ptr [eax]
        mov eax, [esi + 174h]
        cmp dword ptr [eax], 0
        jne assign
        mov ecx, [eax + 8]
        call dword ptr [eax + 4]
        mov eax, [esi + 174h]
        push eax
        call FableFrontEndLeftClick992470FreeControl
        add esp, 4

    assign:
        test edi, edi
        mov [esi + 170h], ebx
        mov [esi + 174h], edi
        je done
        inc dword ptr [edi]

    done:
        pop edi
        pop esi
        pop ebx
        ret 4
    }
}
