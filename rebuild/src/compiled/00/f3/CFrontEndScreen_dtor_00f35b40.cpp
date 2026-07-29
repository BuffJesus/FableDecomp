extern "C" char FableFrontEndScreenF35B40Vtable;
extern "C" char FableFrontEndScreenF35B40BaseVtable;
extern "C" void __cdecl FableFrontEndScreenF35B40FreeControl(void* control);
extern "C" void __fastcall FableFrontEndScreenF35B40BaseDtor(void* base, void*);

extern "C" __declspec(naked) void __fastcall
CFrontEndScreen_dtor_00f35b40(void* screen, void*)
{
    __asm
    {
        push esi
        lea esi, [ecx + 8]
        mov dword ptr [ecx], offset FableFrontEndScreenF35B40Vtable
        mov dword ptr [esi], offset FableFrontEndScreenF35B40BaseVtable
        mov eax, [esi + 8]
        test eax, eax
        je clear_action
        dec dword ptr [eax]
        mov eax, [esi + 8]
        cmp dword ptr [eax], 0
        jne clear_action
        mov ecx, [eax + 8]
        call dword ptr [eax + 4]
        mov eax, [esi + 8]
        push eax
        call FableFrontEndScreenF35B40FreeControl
        add esp, 4

    clear_action:
        mov dword ptr [esi + 4], 0
        mov dword ptr [esi + 8], 0
        mov ecx, esi
        pop esi
        jmp FableFrontEndScreenF35B40BaseDtor
    }
}
