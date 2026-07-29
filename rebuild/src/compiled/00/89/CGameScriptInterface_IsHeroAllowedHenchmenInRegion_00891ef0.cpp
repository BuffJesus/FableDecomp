class CCharString;

class CWorldMap
{
public:
    long GetRegionNumberFromName(const CCharString& regionName);
};

class CGSIHenchmenRegion_State
{
public:
    virtual void Slot00() = 0;
    virtual void Release() = 0;

    long m_RefCount;
    unsigned char m_Pad08[0x81];
    bool m_IsAllowed;
};

struct CGSIHenchmenRegion_Category
{
    unsigned char m_Pad00[0x24];
    CGSIHenchmenRegion_State* m_pState;
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    CGSIHenchmenRegion_Category* GetCategoryWithIndex(long index);
};
}

class CGameScriptInterface
{
public:
    virtual bool IsHeroAllowedHenchmenInRegion(
        const CCharString& regionName) const;
};

__declspec(naked)
bool CGameScriptInterface::IsHeroAllowedHenchmenInRegion(
    const CCharString& regionName) const
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esi + 4]
        mov edx, dword ptr [ecx]
        push eax
        call dword ptr [edx + 34h]
        mov ecx, eax
        call CWorldMap::GetRegionNumberFromName
        test eax, eax
        jle default_allowed
        mov ecx, dword ptr [esi + 4]
        push eax
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 34h]
        mov ecx, eax
        call NInventory::CTCInventoryBase::GetCategoryWithIndex
        mov ecx, dword ptr [eax + 24h]
        test ecx, ecx
        je default_allowed
        mov edx, dword ptr [ecx + 4]
        inc edx
        mov eax, edx
        mov dword ptr [ecx + 4], edx
        push ebx
        mov bl, byte ptr [ecx + 89h]
        dec eax
        mov dword ptr [ecx + 4], eax
        jne retained_state
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 4]
retained_state:
        mov al, bl
        pop ebx
        pop esi
        ret 4
default_allowed:
        mov al, 1
        pop esi
        ret 4
    }
}
