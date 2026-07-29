enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSIMoralityStats
{
public:
    long GetMoralityMax();
    void SetMorality(long morality);

    unsigned char m_Pad00[0x28];
    long          m_Morality; // 0x28
};

struct CKeyPair
{
    ETCInterfaceType  m_Key;   // 0x00
    CGSIMoralityStats* m_Value; // 0x04
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
void __fastcall CGameScriptInterface_GiveHeroMorality(
    const CGameScriptInterface* self,
    float amount)
{
    __asm
    {
        push ecx
        push esi
        mov esi, ecx
        mov ecx, dword ptr [esi + 14h]
        call Sub_ResolveA
        mov ecx, eax
        call Sub_ResolveB
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished

        mov ecx, dword ptr [esi + 14h]
        call Sub_ResolveA
        mov ecx, eax
        call Sub_ResolveB
        test byte ptr [eax + 20h], 10h
        je use_amount_bits

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
        mov esi, dword ptr [eax + 4]
        jmp have_stats
use_amount_bits:
        mov esi, dword ptr [esp + 0Ch]
have_stats:
        mov ecx, esi
        call CGSIMoralityStats::GetMoralityMax
        mov dword ptr [esp + 4], eax
        fild dword ptr [esp + 4]
        fmul dword ptr [esp + 0Ch]
        fstp dword ptr [esp + 0Ch]
        fld dword ptr [esp + 0Ch]
        fistp dword ptr [esp + 4]
        mov ecx, dword ptr [esi + 28h]
        add ecx, dword ptr [esp + 4]
        push ecx
        mov ecx, esi
        call CGSIMoralityStats::SetMorality
finished:
        pop esi
        pop ecx
        ret 4
    }
}
