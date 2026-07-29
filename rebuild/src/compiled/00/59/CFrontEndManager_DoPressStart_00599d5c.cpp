// NUISystem::CFrontEndManager::DoPressStart @ 0x00599D5C
// VC7.1, x86, /O2 /Oy.
//
// Press-start routes an empty profile list to profile creation.  A single
// profile is verified and loaded directly when possible; otherwise the
// profile picker is shown and the profile service is applied.  Every route
// finally binds the main player's joystick and destroys the profile vector.

extern "C" void __fastcall
FableFrontEndPressStartGetProfileNames(void* manager, void*, void* names);
extern "C" void __fastcall
FableFrontEndPressStartGotoNewProfile(void* manager, void*);
extern "C" void* __cdecl
FableFrontEndPressStartGetProfileService();
extern "C" void* __fastcall
FableFrontEndPressStartGetDefaultProfile(
    void* service,
    void*,
    void* string,
    void* profile,
    long flags);
extern "C" unsigned char __fastcall
FableFrontEndPressStartVerifyProfile(
    void* manager,
    void*,
    void* defaultProfile);
extern "C" void __fastcall
FableFrontEndPressStartDestroyString(void* string, void*);
extern "C" void __fastcall
FableFrontEndPressStartLoadProfile(
    void* manager,
    void*,
    void* profile);
extern "C" long** __fastcall
FableFrontEndPressStartResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" void __fastcall
FableFrontEndPressStartGotoNext(
    void* manager,
    void*,
    void* component,
    long transition);
extern "C" void __fastcall
FableFrontEndPressStartGotoProfileMenu(void* manager, void*);
extern "C" void __fastcall
FableFrontEndPressStartApplyProfileService(void* service, void*);
extern "C" void* __cdecl
FableFrontEndPressStartGetJoystickManager();
extern "C" void __fastcall
FableFrontEndPressStartSetMainJoystick(
    void* joystickManager,
    void*,
    long device);
extern "C" void __fastcall
FableFrontEndPressStartDestroyProfileNames(void* names, void*);

extern "C" void* FableFrontEndPressStartInputState;

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_DoPressStart_00599d5c(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 18h
        push ebx
        push esi
        xor ebx, ebx
        push edi
        lea eax, [ebp - 18h]
        push eax
        mov esi, ecx
        mov dword ptr [ebp - 8], ebx
        mov dword ptr [ebp - 18h], ebx
        mov dword ptr [ebp - 14h], ebx
        mov dword ptr [ebp - 10h], ebx
        call FableFrontEndPressStartGetProfileNames
        mov edi, dword ptr [ebp - 18h]
        mov eax, dword ptr [ebp - 14h]
        cmp edi, eax
        jne profilesExist
        mov ecx, esi
        call FableFrontEndPressStartGotoNewProfile
        jmp bindJoystick

profilesExist:
        sub eax, edi
        and eax, 0fffffffch
        cmp eax, 4
        jne verificationFailed
        push ebx
        push edi
        lea eax, [ebp - 0ch]
        push eax
        mov dword ptr [ebp - 8], 1
        call FableFrontEndPressStartGetProfileService
        mov ecx, eax
        call FableFrontEndPressStartGetDefaultProfile
        push eax
        mov ecx, esi
        call FableFrontEndPressStartVerifyProfile
        test al, al
        mov byte ptr [ebp - 1], 1
        jne verificationDone

verificationFailed:
        mov byte ptr [ebp - 1], bl

verificationDone:
        test byte ptr [ebp - 8], 1
        je temporaryDestroyed
        lea ecx, [ebp - 0ch]
        call FableFrontEndPressStartDestroyString

temporaryDestroyed:
        cmp byte ptr [ebp - 1], bl
        mov ecx, esi
        je showProfileMenu
        push edi
        call FableFrontEndPressStartLoadProfile
        lea eax, [ebp - 0ch]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 0ch], ebx
        call FableFrontEndPressStartResolveUsedKey
        push 1
        push dword ptr [eax]
        mov ecx, esi
        call FableFrontEndPressStartGotoNext
        jmp bindJoystick

showProfileMenu:
        call FableFrontEndPressStartGotoProfileMenu
        call FableFrontEndPressStartGetProfileService
        mov byte ptr [eax + 0ch], 1
        call FableFrontEndPressStartGetProfileService
        mov ecx, eax
        call FableFrontEndPressStartApplyProfileService

bindJoystick:
        mov eax, dword ptr [FableFrontEndPressStartInputState]
        push dword ptr [eax + 140h]
        call FableFrontEndPressStartGetJoystickManager
        mov ecx, eax
        call FableFrontEndPressStartSetMainJoystick
        lea ecx, [ebp - 18h]
        call FableFrontEndPressStartDestroyProfileNames
        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
