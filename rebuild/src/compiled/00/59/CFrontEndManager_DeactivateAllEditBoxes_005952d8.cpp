// NUISystem::CFrontEndManager::DeactivateAllEditBoxes @ 0x005952D8
// VC7.1, x86, /O2 /Oy.
//
// A null root selects the current front-end stack component.  The routine
// deactivates components of type 0x25, then recursively visits every child in
// the component's eight-byte child-entry vector.

extern "C" void** __fastcall
FableFrontEndDeactivateEditBoxesStackTop(void* stack, void*);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_DeactivateAllEditBoxes_005952d8(
    void*,
    void*,
    void*)
{
    __asm
    {
        push ebx
        push esi
        mov esi, dword ptr [esp + 0ch]
        push edi
        xor edi, edi
        cmp esi, edi
        mov ebx, ecx
        jne haveComponent
        lea ecx, [ebx + 20h]
        call FableFrontEndDeactivateEditBoxesStackTop
        mov esi, dword ptr [eax]
        cmp esi, edi
        je done

haveComponent:
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 104h]
        cmp eax, 25h
        jne inspectChildren
        mov eax, dword ptr [esi]
        push edi
        mov ecx, esi
        call dword ptr [eax + 258h]

inspectChildren:
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 0d4h]
        mov ecx, dword ptr [eax + 4]
        sub ecx, dword ptr [eax]
        sar ecx, 3
        je done

childLoop:
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 0d4h]
        mov eax, dword ptr [eax]
        push dword ptr [eax + edi*8]
        mov ecx, ebx
        call CFrontEndManager_DeactivateAllEditBoxes_005952d8
        mov eax, dword ptr [esi]
        mov ecx, esi
        inc edi
        call dword ptr [eax + 0d4h]
        mov ecx, dword ptr [eax + 4]
        sub ecx, dword ptr [eax]
        sar ecx, 3
        cmp edi, ecx
        jb childLoop

done:
        pop edi
        pop esi
        pop ebx
        ret 4
    }
}
