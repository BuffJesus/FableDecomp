extern "C" void __cdecl FableFrontEndScreenDtor835330FreeControl(void* control);

extern "C" __declspec(naked) void __fastcall
CFrontEndScreen_dtor_00835330(void* screen, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        push edi
        xor edi, edi
        mov [esi], edi
        mov [esi + 4], edi
        mov eax, [esi + 24h]
        cmp eax, edi
        je clear_action
        dec dword ptr [eax]
        mov eax, [esi + 24h]
        cmp [eax], edi
        jne clear_action
        mov ecx, [eax + 8]
        call dword ptr [eax + 4]
        mov eax, [esi + 24h]
        push eax
        call FableFrontEndScreenDtor835330FreeControl
        add esp, 4

    clear_action:
        mov [esi + 24h], edi
        mov [esi + 20h], edi
        pop edi
        pop esi
        ret
    }
}
