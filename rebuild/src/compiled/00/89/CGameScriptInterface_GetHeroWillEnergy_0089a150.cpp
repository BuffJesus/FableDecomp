enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CGSIWillEnergyComponent
{
    unsigned char m_Pad00[0x58];
    long          m_CurrentEnergy; // 0x58
};

struct CKeyPair
{
    ETCInterfaceType m_Key;   // 0x00
    void*            m_Value; // 0x04
};

struct CVectorMap
{
    CKeyPair* m_pBegin; // 0x00
    CKeyPair* m_pEnd;   // 0x04

    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x20];
    unsigned char m_Flags20;       // 0x20
    unsigned char m_Pad21[0x23];
    CVectorMap    m_InterfaceMap44; // 0x44
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;        // 0x91
};

struct CGameScriptInterface
{
    unsigned char m_Pad00[0x14];
    void*         m_Field14; // 0x14
};

extern "C" void* __fastcall Sub_ResolveA(void* self);
extern "C" CThingOverlay* __fastcall Sub_ResolveB(void* obj);

const ETCInterfaceType kTargetType = (ETCInterfaceType)4;

__declspec(naked)
long __fastcall CGameScriptInterface_GetHeroWillEnergy(
    const CGameScriptInterface* self)
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call Sub_ResolveA
        mov ecx, eax
        call Sub_ResolveB
        test eax, eax
        je invalid
        test byte ptr [eax + 91h], 1
        jne invalid
        test byte ptr [eax + 20h], 10h
        je invalid

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call CVectorMap::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov eax, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 58h]
        pop esi
        pop ecx
        ret
invalid:
        xor eax, eax
        pop ecx
        ret
    }
}
