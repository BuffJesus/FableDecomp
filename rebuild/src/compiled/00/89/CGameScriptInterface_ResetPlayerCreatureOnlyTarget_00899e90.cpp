enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSIOnlyTarget_Component
{
public:
    void SetOnlyTarget(void* pThing);
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
    unsigned int  m_Flags20;       // 0x20
    unsigned char m_Pad24[0x20];
    CVectorMap    m_InterfaceMap44; // 0x44
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;        // 0x91
};

struct CGameScriptContext
{
    unsigned char m_Pad00[0x1c];
    void*         m_Manager; // 0x1c
};

struct CGameScriptInterface
{
    unsigned char       m_Pad00[0x08];
    CGameScriptContext* m_Context; // 0x08
};

extern "C" void* __fastcall Sub_ResolveA(void* self);
extern "C" void* __fastcall Sub_ResolveB(void* obj);

const ETCInterfaceType kTargetType = (ETCInterfaceType)8;

__declspec(naked)
void __fastcall CGameScriptInterface_ResetPlayerCreatureOnlyTarget(
    const CGameScriptInterface* self)
{
    __asm
    {
        push ecx
        mov eax, dword ptr [ecx + 8]
        mov ecx, dword ptr [eax + 1Ch]
        call Sub_ResolveA
        mov ecx, eax
        call Sub_ResolveB
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        mov ecx, dword ptr [eax + 20h]
        test ch, 1
        je finished

        push esi
        lea ecx, [esp + 4]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 8], 8
        call CVectorMap::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 8
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        push 0
        mov ecx, eax
        call CGSIOnlyTarget_Component::SetOnlyTarget
        pop esi
finished:
        pop ecx
        ret
    }
}
