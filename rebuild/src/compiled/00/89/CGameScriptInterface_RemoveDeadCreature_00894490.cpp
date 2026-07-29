class CCharString
{
};

class CGSIRemoveDead_String
{
public:
    void CopyConstruct(const void* pSource);
    void Destruct();
};

class CGSIRemoveDead_SearchTools
{
public:
    bool GetPFirstThing(
        const void* pFilter,
        void** ppThing);
};

class CGSIRemoveDead_Thing
{
public:
    void Kill(bool immediately);
};

extern "C" void* __fastcall
CGSIRemoveDead_AppendCString(
    void* pDestination,
    const CCharString* pBase,
    const char* pSuffix);

class CGameScriptInterface
{
public:
    virtual void RemoveDeadCreature(
        const CCharString& creatureName) const;
};

static const char g_CGSIRemoveDead_Suffix[] =
    "_DEAD_CREATURE";

__declspec(naked)
void CGameScriptInterface::RemoveDeadCreature(
    const CCharString& creatureName) const
{
    __asm
    {
        sub esp, 8
        mov edx, dword ptr [esp + 0Ch]
        push ebx
        push esi
        mov esi, ecx
        push offset g_CGSIRemoveDead_Suffix
        lea ecx, [esp + 10h]
        mov dword ptr [esp + 0Ch], 0
        call CGSIRemoveDead_AppendCString
        push eax
        lea ecx, [esp + 18h]
        call CGSIRemoveDead_String::CopyConstruct
        mov eax, dword ptr [esi + 4]
        mov ecx, dword ptr [eax + 20h]
        lea edx, [esp + 8]
        push edx
        lea eax, [esp + 18h]
        push eax
        call CGSIRemoveDead_SearchTools::GetPFirstThing
        lea ecx, [esp + 14h]
        mov bl, al
        call CGSIRemoveDead_String::Destruct
        lea ecx, [esp + 0Ch]
        call CGSIRemoveDead_String::Destruct
        pop esi
        test bl, bl
        pop ebx
        je not_found
        mov ecx, dword ptr [esp]
        push 0
        call CGSIRemoveDead_Thing::Kill
not_found:
        add esp, 8
        ret 4
    }
}
