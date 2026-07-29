// NUISystem::CFrontEndManager::CreateNewProfile @ 0x0059697A
// VC7.1, x86, /O2 /Oy.
//
// PDB ABI:
//   bool __thiscall CreateNewProfile(CWideString&)
//
// Retail validates storage, disables profile-manager +0x0c, validates the
// candidate name through the default-value string path, assigns it, resets
// and applies profile defaults, transfers the selected inversion setting,
// saves, selects the no-continue main-menu definition, and propagates the
// profile name to screens.  It owns no component stack, transition, sound,
// or intrusive-reference operation; those live in the caller routes.

extern "C" void* __cdecl
FableFrontEndCreateProfileGetProfileManager();
extern "C" unsigned char __fastcall
FableFrontEndCreateProfileHasSpace(void* profileManager, void*);
extern "C" void* __fastcall
FableFrontEndCreateProfileGetDefaultValue(
    void* profileManager,
    void*,
    void* result,
    void* name,
    int create);
extern "C" long __fastcall
FableFrontEndCreateProfileStringLength(void* text, void*);
extern "C" void __fastcall
FableFrontEndCreateProfileDestroyString(void* text, void*);
extern "C" void __fastcall
FableFrontEndCreateProfileAssignName(
    void* profileManager,
    void*,
    void* name,
    int option);
extern "C" void __fastcall
FableFrontEndCreateProfileSetDefaults(void* profileManager, void*);
extern "C" void __fastcall
FableFrontEndCreateProfileApplyValues(void* profileManager, void*);
extern "C" void __fastcall
FableFrontEndCreateProfileSetBowInverted(
    void* profileManager,
    void*,
    int inverted,
    int apply);
extern "C" void __fastcall
FableFrontEndCreateProfileSave(void* profileManager, void*);
extern "C" void __fastcall
FableFrontEndCreateProfileConstructLiteral(
    void* text,
    void*,
    const char* literal,
    int length);
extern "C" void __fastcall
FableFrontEndCreateProfileSetMainMenuName(
    void* manager,
    void*,
    void* text);
extern "C" void __fastcall
FableFrontEndCreateProfileDestroyLiteral(void* text, void*);
extern "C" void __fastcall
FableFrontEndCreateProfileAddNameToScreens(
    void* manager,
    void*,
    void* name);

extern "C" const char
FableFrontEndCreateProfileNoContinueMenuName[];

extern "C" __declspec(naked) unsigned char __fastcall
CFrontEndManager_CreateNewProfile_0059697a(
    void*,
    void*,
    void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push ecx
        push esi
        mov esi, ecx
        mov byte ptr [ebp - 1], 1
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileHasSpace
        test al, al
        je done
        push ebx
        call FableFrontEndCreateProfileGetProfileManager
        push 1
        push dword ptr [ebp + 8]
        mov byte ptr [eax + 0ch], 0
        lea eax, [ebp - 8]
        push eax
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileGetDefaultValue
        mov ecx, eax
        call FableFrontEndCreateProfileStringLength
        mov ebx, eax
        neg ebx
        sbb bl, bl
        lea ecx, [ebp - 8]
        inc bl
        call FableFrontEndCreateProfileDestroyString
        test bl, bl
        pop ebx
        je nameAccepted
        mov byte ptr [ebp - 1], 0

nameAccepted:
        push 0
        push dword ptr [ebp + 8]
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileAssignName
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileSetDefaults
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileApplyValues
        call FableFrontEndCreateProfileGetProfileManager
        cmp byte ptr [eax + 0ch], 0
        je usePrimaryInversion
        mov al, byte ptr [eax + 0d0h]
        jmp inversionReady

usePrimaryInversion:
        movzx eax, byte ptr [eax + 10h]

inversionReady:
        push 1
        push eax
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileSetBowInverted
        call FableFrontEndCreateProfileGetProfileManager
        mov ecx, eax
        call FableFrontEndCreateProfileSave
        push -1
        push offset FableFrontEndCreateProfileNoContinueMenuName
        lea ecx, [ebp - 8]
        call FableFrontEndCreateProfileConstructLiteral
        lea eax, [ebp - 8]
        push eax
        mov ecx, esi
        call FableFrontEndCreateProfileSetMainMenuName
        lea ecx, [ebp - 8]
        call FableFrontEndCreateProfileDestroyLiteral
        push dword ptr [ebp + 8]
        mov ecx, esi
        call FableFrontEndCreateProfileAddNameToScreens
        mov al, byte ptr [ebp - 1]

done:
        pop esi
        leave
        ret 4
    }
}
