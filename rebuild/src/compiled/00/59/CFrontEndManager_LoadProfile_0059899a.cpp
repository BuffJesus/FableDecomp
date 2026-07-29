// NUISystem::CFrontEndManager::LoadProfile @ 0x0059899A
// VC7.1, x86, /O2 /Oy.
//
// Retail clears the save/profile service's +0x0c byte before loading the
// requested profile.  It then enumerates saved games and chooses the main
// menu definition according to whether that vector is empty, publishes the
// profile name to the front-end screens, and destroys both temporaries.

extern "C" void* __cdecl
FableFrontEndLoadProfileGetService();
extern "C" void __fastcall
FableFrontEndLoadProfileLoadSelected(
    void* service,
    void*,
    void* profileName,
    long activate);
extern "C" void __fastcall
FableFrontEndLoadProfileGetSavedGameNames(
    void* manager,
    void*,
    void* names);
extern "C" void __fastcall
FableFrontEndLoadProfileConstructString(
    void* string,
    void*,
    const char* text,
    long length);
extern "C" void __fastcall
FableFrontEndLoadProfileSetMainMenuDefName(
    void* manager,
    void*,
    void* name);
extern "C" void __fastcall
FableFrontEndLoadProfileDestroyString(void* string, void*);
extern "C" void __fastcall
FableFrontEndLoadProfileAddProfileNameToScreens(
    void* manager,
    void*,
    void* profileName);
extern "C" void __fastcall
FableFrontEndLoadProfileDestroyNames(void* names, void*);

extern "C" const char
FableFrontEndLoadProfileNoLiveAwareNoContinue[];
extern "C" const char
FableFrontEndLoadProfileNoLiveAware[];

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_LoadProfile_0059899a(void*, void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 10h
        push esi
        mov esi, ecx
        call FableFrontEndLoadProfileGetService
        push 1
        push dword ptr [ebp + 8]
        mov byte ptr [eax + 0ch], 0
        call FableFrontEndLoadProfileGetService
        mov ecx, eax
        call FableFrontEndLoadProfileLoadSelected
        and dword ptr [ebp - 10h], 0
        and dword ptr [ebp - 0ch], 0
        and dword ptr [ebp - 8], 0
        lea eax, [ebp - 10h]
        push eax
        mov ecx, esi
        call FableFrontEndLoadProfileGetSavedGameNames
        mov eax, dword ptr [ebp - 10h]
        cmp eax, dword ptr [ebp - 0ch]
        lea ecx, [ebp - 4]
        push -1
        jne hasSavedGames
        push offset FableFrontEndLoadProfileNoLiveAwareNoContinue
        jmp constructMenuName

hasSavedGames:
        push offset FableFrontEndLoadProfileNoLiveAware

constructMenuName:
        call FableFrontEndLoadProfileConstructString
        lea eax, [ebp - 4]
        mov ecx, esi
        push eax
        call FableFrontEndLoadProfileSetMainMenuDefName
        lea ecx, [ebp - 4]
        call FableFrontEndLoadProfileDestroyString
        push dword ptr [ebp + 8]
        mov ecx, esi
        call FableFrontEndLoadProfileAddProfileNameToScreens
        lea ecx, [ebp - 10h]
        call FableFrontEndLoadProfileDestroyNames
        pop esi
        leave
        ret 4
    }
}
