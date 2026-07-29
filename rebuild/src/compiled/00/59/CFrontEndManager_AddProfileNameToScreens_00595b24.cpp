// NUISystem::CFrontEndManager::AddProfileNameToScreens @ 0x00595B24
// VC7.1, x86, /O2 /Oy.
//
// Retail prepares one owned profile-name prefix, applies it to nine fixed
// screen/title pairs, destroys each temporary title identifier immediately,
// then destroys the prepared prefix.

extern "C" void __fastcall
FableFrontEndProfileTitlesPreparePrefix(
    void* destination,
    const void* profileName,
    const void* separator);
extern "C" void __fastcall
FableFrontEndProfileTitlesConstructName(
    void* name,
    void*,
    const char* text,
    long length);
extern "C" void __fastcall
FableFrontEndProfileTitlesAddPrefix(
    void* manager,
    void*,
    unsigned long screen,
    void* titleName,
    void* prefix);
extern "C" void __fastcall
FableFrontEndProfileTitlesDestroyName(void* name, void*);
extern "C" void __fastcall
FableFrontEndProfileTitlesDestroyPrefix(void* prefix, void*);

extern "C" const unsigned char
FableFrontEndProfileTitlesSeparator[];
extern "C" const char FableFrontEndProfileTitlesNewGame[];
extern "C" const char FableFrontEndProfileTitlesLoadGame[];
extern "C" const char FableFrontEndProfileTitlesOptions[];
extern "C" const char FableFrontEndProfileTitlesGameOptions[];
extern "C" const char FableFrontEndProfileTitlesVideo[];
extern "C" const char FableFrontEndProfileTitlesScoreboard[];
extern "C" const char FableFrontEndProfileTitlesRedefineKeys[];
extern "C" const char FableFrontEndProfileTitlesAudio[];

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_AddProfileNameToScreens_00595b24(
    void*,
    void*,
    const void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        mov edx, dword ptr [ebp + 8]
        push ebx
        push esi
        push edi
        mov esi, ecx
        push offset FableFrontEndProfileTitlesSeparator
        lea ecx, [ebp + 8]
        call FableFrontEndProfileTitlesPreparePrefix
        or edi, -1

        push edi
        push offset FableFrontEndProfileTitlesNewGame
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 0
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push offset FableFrontEndProfileTitlesLoadGame
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 0
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        mov ebx, offset FableFrontEndProfileTitlesOptions
        push ebx
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 18h
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push ebx
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 1
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push offset FableFrontEndProfileTitlesGameOptions
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 1
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push offset FableFrontEndProfileTitlesVideo
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 5
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push offset FableFrontEndProfileTitlesScoreboard
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 19h
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push offset FableFrontEndProfileTitlesRedefineKeys
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 16h
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        push edi
        push offset FableFrontEndProfileTitlesAudio
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesConstructName
        lea eax, [ebp + 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        push 4
        mov ecx, esi
        call FableFrontEndProfileTitlesAddPrefix
        lea ecx, [ebp - 4]
        call FableFrontEndProfileTitlesDestroyName

        lea ecx, [ebp + 8]
        call FableFrontEndProfileTitlesDestroyPrefix
        pop edi
        pop esi
        pop ebx
        leave
        ret 4
    }
}
