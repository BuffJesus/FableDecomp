// Recovered frontend profile-list refresh helper @ 0x00495830.
// The shared manifest's CEngineLandscapeMap::RenderBackground label is stale.

class CFrontendProfileRange_00495830
{
public:
    void* m_Begin;
    void* m_End;
    void* m_Capacity;

    void Clear(void*, void*);
    void Refresh();
};

class CFrontendProfileManager_00495830
{
public:
    void PopulateList(
        CFrontendProfileRange_00495830*,
        CFrontendProfileRange_00495830*);
};

extern CFrontendProfileManager_00495830* __cdecl
GetFrontendProfileManager_00495830();
extern void __cdecl FrontendProfileListFree_00495830(void*);

__declspec(naked) void CFrontendProfileRange_00495830::Refresh()
{
    __asm
    {
        sub esp, 0ch
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 4]
        mov ecx, dword ptr [esi]
        push eax
        push ecx
        mov ecx, esi
        call CFrontendProfileRange_00495830::Clear
        lea edx, [esp + 4]
        push edx
        push esi
        mov dword ptr [esp + 0ch], 0
        mov dword ptr [esp + 10h], 0
        mov dword ptr [esp + 14h], 0
        call GetFrontendProfileManager_00495830
        mov ecx, eax
        call CFrontendProfileManager_00495830::PopulateList
        mov eax, dword ptr [esp + 4]
        test eax, eax
        pop esi
        je temporary_done
        push eax
        call FrontendProfileListFree_00495830
        add esp, 4

    temporary_done:
        add esp, 0ch
        ret
    }
}
