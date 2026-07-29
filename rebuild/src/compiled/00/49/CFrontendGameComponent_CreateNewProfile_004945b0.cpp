// CFrontendGameComponent::CreateNewProfile @ 0x004945B0.

class CFrontendProfileString_004945b0
{
public:
    long m_Value;

    CFrontendProfileString_004945b0();
    CFrontendProfileString_004945b0(
        const CFrontendProfileString_004945b0&);
    ~CFrontendProfileString_004945b0();

    void Construct();
    void Copy(const CFrontendProfileString_004945b0*);
    void Destroy();
    void __cdecl Format(const char*, ...);
};

class CFrontendProfileManager_004945b0
{
public:
    void CreateProfileResult(
        CFrontendProfileString_004945b0*,
        const CFrontendProfileString_004945b0*,
        long);
    void SelectProfile(const CFrontendProfileString_004945b0*, long);
    void FinaliseProfiles();
};

extern const char g_FrontendProfileFormat_004945b0[];
extern const unsigned short g_FrontendProfilePrefix_004945b0[];

extern void __fastcall BuildFrontendProfileName_004945b0(
    CFrontendProfileString_004945b0*,
    const unsigned short*,
    const CFrontendProfileString_004945b0*);

extern CFrontendProfileManager_004945b0* __cdecl
GetFrontendProfileManager_004945b0();

class CFrontendGameComponent_004945b0
{
    unsigned char m_Pad0000[0x1e0c];
    CFrontendProfileString_004945b0* m_ProfileBegin;
    CFrontendProfileString_004945b0* m_ProfileEnd;

public:
    CFrontendProfileString_004945b0 CreateNewProfile();
};

__declspec(naked)
CFrontendProfileString_004945b0
CFrontendGameComponent_004945b0::CreateNewProfile()
{
    __asm
    {
        mov eax, dword ptr [ecx + 1e0ch]
        sub esp, 0ch
        push esi
        mov esi, dword ptr [ecx + 1e10h]
        sub esi, eax
        sar esi, 2
        lea ecx, [esp + 8]
        inc esi
        call CFrontendProfileString_004945b0::Construct
        push esi
        lea eax, [esp + 0ch]
        push offset g_FrontendProfileFormat_004945b0
        push eax
        call CFrontendProfileString_004945b0::Format
        add esp, 0ch
        lea ecx, [esp + 8]
        push ecx
        lea ecx, [esp + 10h]
        call CFrontendProfileString_004945b0::Copy
        lea ecx, [esp + 8]
        call CFrontendProfileString_004945b0::Destroy
        lea edx, [esp + 0ch]
        push edx
        mov edx, offset g_FrontendProfilePrefix_004945b0
        lea ecx, [esp + 8]
        call BuildFrontendProfileName_004945b0
        lea ecx, [esp + 0ch]
        call CFrontendProfileString_004945b0::Destroy
        push 1
        lea eax, [esp + 8]
        push eax
        lea ecx, [esp + 14h]
        push ecx
        call GetFrontendProfileManager_004945b0
        mov ecx, eax
        call CFrontendProfileManager_004945b0::CreateProfileResult
        lea ecx, [esp + 0ch]
        call CFrontendProfileString_004945b0::Destroy
        push 0
        lea edx, [esp + 8]
        push edx
        call GetFrontendProfileManager_004945b0
        mov ecx, eax
        call CFrontendProfileManager_004945b0::SelectProfile
        call GetFrontendProfileManager_004945b0
        mov ecx, eax
        call CFrontendProfileManager_004945b0::FinaliseProfiles
        mov esi, dword ptr [esp + 14h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        call CFrontendProfileString_004945b0::Copy
        lea ecx, [esp + 4]
        call CFrontendProfileString_004945b0::Destroy
        mov eax, esi
        pop esi
        add esp, 0ch
        ret 4
    }
}
