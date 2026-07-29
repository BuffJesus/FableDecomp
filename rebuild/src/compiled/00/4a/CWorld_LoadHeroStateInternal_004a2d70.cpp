// CWorld::LoadHeroStateInternal @ 0x004A2D70.
//
// The companion hero-state filename is saveName + ".hs".  A missing
// companion is accepted.  When present, it is loaded through the normal
// game-state loader with the world's hero-state latch at +0x102 set only
// for the duration of that call.  A successfully loaded autosave companion
// is then removed from the drive manager.

extern "C" const unsigned short CWorld_HeroStateSuffix_004a2d70[];
extern "C" void* CWorld_DriveManager_004a2d70;

extern "C" void* __fastcall
CWorld_AppendWideSuffix_004a2d70(
    void* destination,
    const void* value,
    const unsigned short* suffix);
extern "C" void* __cdecl CWorld_GetProfileManager_004a2d70();
extern "C" void* __fastcall
CWorld_GetPathNameFromFileName_004a2d70(
    void* manager,
    void*,
    void* destination,
    const void* fileName,
    int directorySelector);
extern "C" bool __fastcall CWorld_PathExists_004a2d70(const void* path, void*);
extern "C" bool __fastcall
CWorld_LoadGameStateInternal_004a2d70(
    void* world,
    void*,
    const void* path,
    int showProgress);
extern "C" void* __fastcall
CWorld_GetAutoSaveCheckPointPathName_004a2d70(
    void* manager,
    void*,
    void* destination);
extern "C" void __fastcall
CWorld_DestroyWideString_004a2d70(void* value, void*);
extern "C" int __cdecl
CWorld_CompareWideCharacters_004a2d70(
    const unsigned short* left,
    const unsigned short* right,
    unsigned long count);
extern "C" int __fastcall
CWorld_DeleteHeroStatePath_004a2d70(
    void* driveManager,
    void*,
    const void* path);

extern "C" __declspec(naked) bool __fastcall
CWorld_LoadHeroStateInternal_004a2d70(
    void*,
    void*,
    const void*)
{
    __asm
    {
        mov edx, dword ptr [esp + 4]
        sub esp, 0Ch
        push esi
        mov esi, ecx
        push offset CWorld_HeroStateSuffix_004a2d70
        lea ecx, dword ptr [esp + 0Ch]
        call CWorld_AppendWideSuffix_004a2d70
        push 0
        lea eax, dword ptr [esp + 0Ch]
        push eax
        lea ecx, dword ptr [esp + 1Ch]
        push ecx
        call CWorld_GetProfileManager_004a2d70
        mov ecx, eax
        call CWorld_GetPathNameFromFileName_004a2d70
        lea ecx, dword ptr [esp + 14h]
        call CWorld_PathExists_004a2d70
        test al, al
        je hero_state_missing

        push ebx
        push 0
        lea edx, dword ptr [esp + 1Ch]
        push edx
        mov ecx, esi
        mov byte ptr [esi + 102h], 1
        call CWorld_LoadGameStateInternal_004a2d70
        mov bl, al
        test bl, bl
        mov byte ptr [esi + 102h], 0
        je finish_loaded_path

        push offset CWorld_HeroStateSuffix_004a2d70
        lea eax, dword ptr [esp + 14h]
        push eax
        call CWorld_GetProfileManager_004a2d70
        mov ecx, eax
        call CWorld_GetAutoSaveCheckPointPathName_004a2d70
        mov edx, eax
        lea ecx, dword ptr [esp + 0Ch]
        call CWorld_AppendWideSuffix_004a2d70
        lea ecx, dword ptr [esp + 10h]
        call CWorld_DestroyWideString_004a2d70

        mov eax, dword ptr [esp + 18h]
        mov ecx, dword ptr [esp + 8]
        cmp eax, ecx
        je delete_autosave_companion
        test eax, eax
        je destroy_autosave_name
        test ecx, ecx
        je destroy_autosave_name
        mov edx, dword ptr [ecx]
        mov esi, dword ptr [eax]
        mov eax, dword ptr [eax + 4]
        mov ecx, dword ptr [ecx + 4]
        sub eax, esi
        sub ecx, edx
        sar eax, 1
        sar ecx, 1
        cmp eax, ecx
        jne destroy_autosave_name
        push eax
        push edx
        push esi
        call CWorld_CompareWideCharacters_004a2d70
        add esp, 0Ch
        test eax, eax
        jne destroy_autosave_name

    delete_autosave_companion:
        mov ecx, dword ptr [CWorld_DriveManager_004a2d70]
        lea edx, dword ptr [esp + 8]
        push edx
        call CWorld_DeleteHeroStatePath_004a2d70

    destroy_autosave_name:
        lea ecx, dword ptr [esp + 8]
        call CWorld_DestroyWideString_004a2d70

    finish_loaded_path:
        lea ecx, dword ptr [esp + 18h]
        call CWorld_DestroyWideString_004a2d70
        lea ecx, dword ptr [esp + 0Ch]
        call CWorld_DestroyWideString_004a2d70
        mov al, bl
        pop ebx
        pop esi
        add esp, 0Ch
        ret 4

    hero_state_missing:
        lea ecx, dword ptr [esp + 14h]
        call CWorld_DestroyWideString_004a2d70
        lea ecx, dword ptr [esp + 8]
        call CWorld_DestroyWideString_004a2d70
        mov al, 1
        pop esi
        add esp, 0Ch
        ret 4
    }
}
