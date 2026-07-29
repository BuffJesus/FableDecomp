// NUISystem::CFrontEndManager::GotoNewProfileScreen @ 0x00595845
// VC7.1, x86, /O2 /Oy.
//
// Retail enables profile-manager +0x0c and frontend +0x64/+0xa0, constructs
// a temporary profile-name vector, plays forward sound +0x1a0 with intrusive
// release, transitions the current stack top to state 6, transfers its +4
// ownership handle, then destroys the temporary vector.  It does not push or
// publish a new screen and does not construct the virtual keyboard here.

extern "C" void* __cdecl
FableFrontEndGotoNewProfileGetProfileManager();
extern "C" void __fastcall
FableFrontEndGotoNewProfileGetNames(
    void* manager,
    void*,
    void* names);
extern "C" void* __cdecl
FableFrontEndGotoNewProfileGetManager();
extern "C" void** __fastcall
FableFrontEndGotoNewProfileStackTop(void* stack, void*);
extern "C" void __fastcall
FableFrontEndGotoNewProfileDestroyNames(void* names, void*);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GotoNewProfileScreen_00595845(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 10h
        push ebx
        push esi
        push edi
        mov esi, ecx
        call FableFrontEndGotoNewProfileGetProfileManager
        mov byte ptr [eax + 0ch], 1
        xor eax, eax
        mov dword ptr [ebp - 10h], eax
        mov dword ptr [ebp - 0ch], eax
        mov dword ptr [ebp - 8], eax
        lea eax, [ebp - 10h]
        push eax
        mov ecx, esi
        call FableFrontEndGotoNewProfileGetNames
        mov byte ptr [esi + 64h], 1
        mov byte ptr [esi + 0a0h], 1
        call FableFrontEndGotoNewProfileGetManager
        mov edi, eax
        call FableFrontEndGotoNewProfileGetManager
        lea ecx, [ebp - 4]
        mov ebx, eax
        mov eax, dword ptr [edi]
        push ecx
        mov ecx, edi
        call dword ptr [eax + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [ebx]
        push 1
        add eax, 1a0h
        push eax
        mov ecx, ebx
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [ebp - 4]
        test eax, eax
        je soundReleased
        mov ecx, eax
        add eax, 4
        dec dword ptr [eax]
        cmp dword ptr [eax], 0
        jne soundReleased
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 4]

soundReleased:
        lea ecx, [esi + 20h]
        call FableFrontEndGotoNewProfileStackTop
        mov esi, dword ptr [eax]
        mov eax, dword ptr [esi]
        push 6
        mov ecx, esi
        call dword ptr [eax + 0c0h]
        call FableFrontEndGotoNewProfileGetManager
        mov edx, dword ptr [eax]
        add esi, 4
        push esi
        mov ecx, eax
        call dword ptr [edx + 14h]
        lea ecx, [ebp - 10h]
        call FableFrontEndGotoNewProfileDestroyNames
        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
