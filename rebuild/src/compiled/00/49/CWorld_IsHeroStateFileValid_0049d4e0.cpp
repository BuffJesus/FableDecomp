// CWorld::IsHeroStateFileValid @ 0x0049D4E0.
//
// Retail appends ".hs" to the selected save name, resolves that filename
// through the active profile directory, and returns whether the resulting
// companion hero-state path exists.  Both temporary wide strings are
// destroyed in construction order's reverse.

extern "C" const unsigned short CWorld_HeroStateSuffix_0049d4e0[];

extern "C" void* __fastcall
CWorld_AppendHeroStateSuffix_0049d4e0(
    void* destination,
    const void* saveName,
    const unsigned short* suffix);

extern "C" void* __cdecl CWorld_GetProfileManager_0049d4e0();

extern "C" void* __fastcall
CWorld_GetPathNameFromFileName_0049d4e0(
    void* profileManager,
    void*,
    void* destination,
    const void* fileName,
    int directorySelector);

extern "C" void __fastcall
CWorld_DestroyWideString_0049d4e0(void* stringValue, void*);

extern "C" bool __fastcall
CWorld_PathExists_0049d4e0(const void* path, void*);

extern "C" __declspec(naked) bool __fastcall
CWorld_IsHeroStateFileValid_0049d4e0(
    void*,
    void*,
    const void*)
{
    __asm
    {
        push ecx
        mov edx, dword ptr [esp + 8]
        push ebx
        push 0
        push offset CWorld_HeroStateSuffix_0049d4e0
        lea ecx, dword ptr [esp + 0Ch]
        call CWorld_AppendHeroStateSuffix_0049d4e0
        push eax
        lea eax, dword ptr [esp + 14h]
        push eax
        call CWorld_GetProfileManager_0049d4e0
        mov ecx, eax
        call CWorld_GetPathNameFromFileName_0049d4e0
        lea ecx, dword ptr [esp + 4]
        call CWorld_DestroyWideString_0049d4e0
        lea ecx, dword ptr [esp + 0Ch]
        call CWorld_PathExists_0049d4e0
        lea ecx, dword ptr [esp + 0Ch]
        mov bl, al
        call CWorld_DestroyWideString_0049d4e0
        mov al, bl
        pop ebx
        pop ecx
        ret 4
    }
}
