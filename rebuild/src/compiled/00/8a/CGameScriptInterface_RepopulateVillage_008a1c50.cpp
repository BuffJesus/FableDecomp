#include <stddef.h>

class CGameScriptInterface;
class CScriptThing;
class CThing;
class CWorldMap;

enum ETCInterfaceType
{
};

struct TCInterfaceRecord
{
    ETCInterfaceType m_Key;
    void* m_Value;
};

struct TCInterfaceMapOverlay
{
    void* m_pBegin;
    TCInterfaceRecord* m_pEnd;

    TCInterfaceRecord* LowerBound(const ETCInterfaceType* key);
};

class CWorld
{
public:
    virtual void m_Virtual00() = 0;
    virtual void m_Virtual04() = 0;
    virtual void m_Virtual08() = 0;
    virtual void m_Virtual0C() = 0;
    virtual void m_Virtual10() = 0;
    virtual void m_Virtual14() = 0;
    virtual void m_Virtual18() = 0;
    virtual void m_Virtual1C() = 0;
    virtual void m_Virtual20() = 0;
    virtual void m_Virtual24() = 0;
    virtual void m_Virtual28() = 0;
    virtual void m_Virtual2C() = 0;
    virtual CWorldMap* GetWorldMap(CThing* villageThing) = 0;
};

class CWorldMap
{
public:
    void PopulateRegionWithVillagers();
};

class CGameScriptInterface
{
public:
    void RepopulateVillage(const CScriptThing& thing) const;
};

extern "C" TCInterfaceRecord* LowerBound0040F020(
    void* map, const ETCInterfaceType* key);
extern "C" void PopulateRegionWithVillagers00502e90(CWorldMap* worldMap);

struct CScriptThingVTableOverlay
{
    unsigned char m_Pad00[0x2C];
    CThing* (__fastcall* m_GetThing)(const CScriptThing* pThis);
    unsigned char m_Pad30[0xFC];
    char (__fastcall* m_IsValidThing)(const CScriptThing* pThis);
};

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable;
};

struct CGameScriptInterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CWorld* m_pWorld;
};

struct VillageThingOverlay
{
    unsigned char m_Pad00[0x24];
    unsigned char m_Flags24;
    unsigned char m_Pad25[0x1F];
    TCInterfaceMapOverlay m_InterfaceMap44;
};

void __declspec(naked) CGameScriptInterface::RepopulateVillage(
    const CScriptThing& thing) const
{
    __asm
    {
        push ebx
        push esi
        mov esi, dword ptr [esp+0Ch]
        mov eax, dword ptr [esi]
        mov ebx, ecx
        mov ecx, esi
        call dword ptr [eax+12Ch]
        test al, al
        je RepopulateVillage_invalid
        mov edx, dword ptr [esi]
        push edi
        mov ecx, esi
        call dword ptr [edx+2Ch]
        mov edi, eax
        test byte ptr [edi+24h], 4
        je RepopulateVillage_with_edi
        lea eax, [esp+10h]
        lea esi, [edi+44h]
        push eax
        mov ecx, esi
        mov dword ptr [esp+14h], 22h
        call LowerBound0040F020
        mov esi, dword ptr [esi+4]
        cmp eax, esi
        je RepopulateVillage_end
        cmp dword ptr [eax], 22h
        jle RepopulateVillage_record
RepopulateVillage_end:
        mov eax, esi
RepopulateVillage_record:
        mov ecx, dword ptr [eax+4]
        test ecx, ecx
        je RepopulateVillage_with_edi
        mov ecx, dword ptr [ebx+4]
        mov edx, dword ptr [ecx]
        push edi
        call dword ptr [edx+30h]
        mov ecx, eax
        call PopulateRegionWithVillagers00502e90
RepopulateVillage_with_edi:
        pop edi
RepopulateVillage_invalid:
        pop esi
        pop ebx
        ret 4
    }
}
