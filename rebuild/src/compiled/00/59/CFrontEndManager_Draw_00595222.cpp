// NUISystem::CFrontEndManager::Draw @ 0x00595222
// VC7.1, x86, /O2 /Oy.
//
// Retail walks the used-component node list.  Each non-null component is
// drawn with the supplied engine and primitive handle plus three zero flags.

extern "C" void* __cdecl
FableFrontEndDrawNextNode(void* node);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_Draw_00595222(
    void*,
    void*,
    void*,
    void*)
{
    __asm
    {
        push ebx
        mov ebx, ecx
        mov eax, dword ptr [ebx + 54h]
        push esi
        mov esi, dword ptr [eax + 8]
        cmp esi, eax
        je done

drawNode:
        mov ecx, dword ptr [esi + 14h]
        test ecx, ecx
        je advance
        mov eax, dword ptr [ecx]
        push 0
        push 0
        push 0
        push dword ptr [esp + 1ch]
        push dword ptr [esp + 1ch]
        call dword ptr [eax + 8]

advance:
        push esi
        call FableFrontEndDrawNextNode
        mov esi, eax
        cmp esi, dword ptr [ebx + 54h]
        pop ecx
        jne drawNode

done:
        pop esi
        pop ebx
        ret 8
    }
}
