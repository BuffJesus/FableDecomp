// CFrontendGameComponent::ChooseProfileNumber @ 0x004962C0.

class CFrontendProfileValue_004962c0
{
public:
    long m_Value;

    CFrontendProfileValue_004962c0();
    CFrontendProfileValue_004962c0(
        const CFrontendProfileValue_004962c0&);
    ~CFrontendProfileValue_004962c0();
    void Destroy();
};

class CFrontendProfileActionManager_004962c0
{
public:
    void* GetDefaultProfileAction();
    void DispatchProfileAction(void*, long);
};

extern CFrontendProfileActionManager_004962c0* __cdecl
GetFrontendProfileActionManager_004962c0();

class CFrontendGameComponent_004962c0
{
    unsigned char m_Data[0x1e48];

    CFrontendProfileValue_004962c0 CreateNewProfile();
    void PopulateProfiles();

public:
    void ChooseProfileNumber(long);
};

__declspec(naked)
void CFrontendGameComponent_004962c0::ChooseProfileNumber(long)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 1e18h]
        mov ecx, dword ptr [esp + 8]
        lea eax, [eax + ecx - 1]
        cmp eax, 0bh
        push edi
        jne check_create_profile
        lea edx, [esp + 0ch]
        push edx
        call GetFrontendProfileActionManager_004962c0
        mov ecx, eax
        call CFrontendProfileActionManager_004962c0::GetDefaultProfileAction
        mov edi, 1
        push edi
        push eax
        call GetFrontendProfileActionManager_004962c0
        mov ecx, eax
        call CFrontendProfileActionManager_004962c0::DispatchProfileAction
        lea ecx, [esp + 0ch]
        call CFrontendProfileValue_004962c0::Destroy
        jmp selection_made

    check_create_profile:
        cmp eax, 0ch
        jne check_existing_profile
        lea eax, [esp + 0ch]
        push eax
        mov ecx, esi
        call CFrontendGameComponent_004962c0::CreateNewProfile
        push 0
        push eax
        call GetFrontendProfileActionManager_004962c0
        mov ecx, eax
        call CFrontendProfileActionManager_004962c0::DispatchProfileAction
        lea ecx, [esp + 0ch]
        call CFrontendProfileValue_004962c0::Destroy
        mov edi, 1
        jmp selection_made

    check_existing_profile:
        test eax, eax
        jl selection_invalid
        mov ecx, dword ptr [esi + 1e10h]
        mov edx, dword ptr [esi + 1e0ch]
        sub ecx, edx
        sar ecx, 2
        cmp eax, ecx
        jae selection_invalid
        mov ecx, edx
        mov edi, 1
        push edi
        lea edx, [ecx + eax*4]
        push edx
        call GetFrontendProfileActionManager_004962c0
        mov ecx, eax
        call CFrontendProfileActionManager_004962c0::DispatchProfileAction

    selection_made:
        mov dword ptr [esi + 1ch], edi
        mov dword ptr [esi + 1e44h], edi
        xor eax, eax
        lea edi, [esi + 168h]
        mov ecx, 20h
        rep stos dword ptr es:[edi]
        cmp dword ptr [esi + 1ch], 0ah
        mov dword ptr [esi + 1e8h], 0
        jne selection_invalid
        mov ecx, esi
        call CFrontendGameComponent_004962c0::PopulateProfiles

    selection_invalid:
        pop edi
        pop esi
        ret 4
    }
}
