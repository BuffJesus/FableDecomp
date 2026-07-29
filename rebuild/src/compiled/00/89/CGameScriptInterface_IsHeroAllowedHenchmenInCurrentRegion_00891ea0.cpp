class CIDrawEnvironment;

class CWorld
{
public:
    CIDrawEnvironment* DrawGetEnvironment();
};

struct CGSIHenchmenCurrentRegion_InterfaceSource
{
    void* m_pVftable;
};

class CGSIHenchmenCurrentRegion_State
{
public:
    virtual void Slot00() = 0;
    virtual void Release() = 0;

    volatile long m_RefCount;
    unsigned char m_Pad08[0x81];
    bool m_IsAllowed;
};

struct CGSIHenchmenCurrentRegion_Category
{
    unsigned char m_Pad00[0x24];
    CGSIHenchmenCurrentRegion_State* m_pState;
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    CGSIHenchmenCurrentRegion_Category* GetCategoryWithIndex(long index);
};
}

struct CGSIHenchmenCurrentRegion_InterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CGSIHenchmenCurrentRegion_InterfaceSource* m_pInterfaceSource;
};

class CGameScriptInterface
{
public:
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const;
};

__declspec(naked)
bool CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion() const
{
    __asm
    {
        mov eax, ecx
        mov ecx, dword ptr [eax + 4]
        mov eax, dword ptr [ecx]
        push esi
        mov esi, ecx
        call dword ptr [eax + 34h]
        mov ecx, eax
        call CWorld::DrawGetEnvironment
        mov edx, dword ptr [esi]
        push eax
        mov ecx, esi
        call dword ptr [edx + 34h]
        mov ecx, eax
        call NInventory::CTCInventoryBase::GetCategoryWithIndex
        mov ecx, dword ptr [eax + 24h]
        test ecx, ecx
        pop esi
        je missing_state
        mov edx, dword ptr [ecx + 4]
        inc edx
        mov eax, edx
        mov dword ptr [ecx + 4], edx
        push ebx
        mov bl, byte ptr [ecx + 89h]
        dec eax
        mov dword ptr [ecx + 4], eax
        jne retained_state
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 4]
retained_state:
        mov al, bl
        pop ebx
        ret
missing_state:
        mov al, 1
        ret
    }
}
