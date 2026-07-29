enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CScriptThing;
struct CThingOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CThingOverlay* (__fastcall* m_Resolve)(CScriptThing* self); // 0x2c
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self); // 0x12c
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;
};

struct CGSIStealableComponent
{
    unsigned char m_Pad00[0x0d];
    bool          m_IsStealable; // 0x0d
};

struct CKeyPair
{
    ETCInterfaceType      m_Key;   // 0x00
    CGSIStealableComponent* m_Value; // 0x04
};

struct CVectorMap
{
    CKeyPair* m_pBegin; // 0x00
    CKeyPair* m_pEnd;   // 0x04

    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x28];
    unsigned int  m_Flags28;       // 0x28
    unsigned char m_Pad2C[0x18];
    CVectorMap    m_InterfaceMap44; // 0x44
};

struct CGameScriptInterface
{
};

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x54;

__declspec(naked)
bool __fastcall CGameScriptInterface_IsEntityStealable(
    const CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing)
{
    __asm
    {
        push esi
        mov esi, dword ptr [esp + 8]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 12Ch]
        test al, al
        je false_result
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2Ch]
        test dword ptr [eax + 28h], 100000h
        je false_result

        lea esi, [eax + 44h]
        lea eax, [esp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 54h
        call CVectorMap::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 54h
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov eax, dword ptr [eax + 4]
        mov cl, byte ptr [eax + 0Dh]
        test cl, cl
        je false_result
        mov al, 1
        pop esi
        ret 4
false_result:
        xor al, al
        pop esi
        ret 4
    }
}
