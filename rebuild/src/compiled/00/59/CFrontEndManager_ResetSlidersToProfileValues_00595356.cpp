// NUISystem::CFrontEndManager::ResetSlidersToProfileValues @ 0x00595356
// VC7.1, x86, /O2 /Oy.
//
// Retail collects components for key 0x0f, applies their profile adjustment,
// clears the temporary list, then repeats for key 0x10 with the second
// component-specific adjustment routine before destroying the list.

extern "C" void __fastcall
FableFrontEndResetSlidersInitialiseList(
    void* list,
    void*,
    void* allocator);
extern "C" void** __fastcall
FableFrontEndResetSlidersStackTop(void* stack, void*);
extern "C" void __fastcall
FableFrontEndResetSlidersAdjustGroup15(void* component, void*);
extern "C" void __fastcall
FableFrontEndResetSlidersClearList(void* list, void*);
extern "C" void __fastcall
FableFrontEndResetSlidersAdjustGroup16(void* component, void*);
extern "C" void __fastcall
FableFrontEndResetSlidersDestroyList(void* list, void*);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_ResetSlidersToProfileValues_00595356(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push ecx
        push esi
        push edi
        lea eax, [ebp - 1]
        mov esi, ecx
        push eax
        lea ecx, [ebp - 8]
        call FableFrontEndResetSlidersInitialiseList
        lea edi, [esi + 20h]
        mov ecx, edi
        call FableFrontEndResetSlidersStackTop
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 8]
        push edx
        push 0fh
        call dword ptr [eax + 18h]
        mov eax, dword ptr [ebp - 8]
        mov esi, dword ptr [eax]
        cmp esi, eax
        je firstPassDone

firstPass:
        mov ecx, dword ptr [esi + 8]
        test ecx, ecx
        je nextFirst
        call FableFrontEndResetSlidersAdjustGroup15

nextFirst:
        mov esi, dword ptr [esi]
        cmp esi, dword ptr [ebp - 8]
        jne firstPass

firstPassDone:
        lea ecx, [ebp - 8]
        call FableFrontEndResetSlidersClearList
        mov ecx, edi
        call FableFrontEndResetSlidersStackTop
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 8]
        push edx
        push 10h
        call dword ptr [eax + 18h]
        mov eax, dword ptr [ebp - 8]
        mov esi, dword ptr [eax]
        cmp esi, eax
        je secondPassDone

secondPass:
        mov ecx, dword ptr [esi + 8]
        test ecx, ecx
        je nextSecond
        call FableFrontEndResetSlidersAdjustGroup16

nextSecond:
        mov esi, dword ptr [esi]
        cmp esi, dword ptr [ebp - 8]
        jne secondPass

secondPassDone:
        lea ecx, [ebp - 8]
        call FableFrontEndResetSlidersDestroyList
        pop edi
        pop esi
        leave
        ret
    }
}
