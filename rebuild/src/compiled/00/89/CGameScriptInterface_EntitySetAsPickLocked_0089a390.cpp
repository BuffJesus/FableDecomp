enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CScriptThing;
struct CEntityOverlay;
struct CHeroOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CEntityOverlay* (__fastcall* m_Resolve)(CScriptThing* self); // 0x2c
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self); // 0x12c
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;
};

class CGSIPickLockedEvent
{
public:
    CGSIPickLockedEvent* Initialize(CHeroOverlay* pHero);
};

class CGSIEntityEvents
{
public:
    void AddEvent(CGSIPickLockedEvent* pEvent);
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

struct CEntityOverlay
{
    unsigned char m_Pad00[0x28];
    unsigned int  m_Flags28;       // 0x28
    unsigned char m_Pad2C[0x18];
    CVectorMap    m_InterfaceMap44; // 0x44
};

struct CHeroOverlay
{
    unsigned char m_Pad00[0x91];
    unsigned char m_Flags91; // 0x91
};

struct CMessageContext
{
    unsigned char m_Pad00[0x60];
    void*         m_MessageManager; // 0x60
};

struct CGameScriptInterface
{
    void**          m_pVTable;       // 0x00
    CMessageContext* m_MessageContext; // 0x04
    unsigned char   m_Pad08[0x0c];
    void*           m_Field14;       // 0x14
};

extern "C" void __stdcall Sub_AddMessage(
    long type,
    long value,
    void* pVector,
    long zero1,
    long zero2,
    long zero3,
    long zero4,
    CEntityOverlay* pEntity);
extern "C" void* __fastcall Sub_ResolveA(void* self);
extern "C" CHeroOverlay* __fastcall Sub_ResolveB(void* obj);
extern "C" void* __cdecl Sub_Allocate(unsigned long size);

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x42;

__declspec(naked)
void __fastcall CGameScriptInterface_EntitySetAsPickLocked(
    CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing)
{
    __asm
    {
        sub esp, 0Ch
        push esi
        push edi
        mov edi, dword ptr [esp + 18h]
        mov eax, dword ptr [edi]
        mov esi, ecx
        mov ecx, edi
        call dword ptr [eax + 12Ch]
        test al, al
        je unavailable

        mov edx, dword ptr [esi]
        push ebx
        push edi
        mov ecx, esi
        call dword ptr [edx + 6A4h]
        mov eax, dword ptr [esi]
        push edi
        mov ecx, esi
        call dword ptr [eax + 6B4h]
        mov edx, dword ptr [edi]
        mov ecx, edi
        call dword ptr [edx + 2Ch]
        mov ecx, dword ptr [esi + 4]
        mov edi, eax

        push edi
        push 0
        push 0
        push 0
        push 0
        lea eax, [esp + 20h]
        push eax
        push 32h
        mov dword ptr [esp + 28h], 0
        mov dword ptr [esp + 2Ch], 0
        mov dword ptr [esp + 30h], 0
        mov ecx, dword ptr [ecx + 60h]
        push 46h
        call Sub_AddMessage

        mov ecx, dword ptr [esi + 14h]
        call Sub_ResolveA
        mov ecx, eax
        call Sub_ResolveB
        mov ebx, eax
        test ebx, ebx
        je finished
        test byte ptr [ebx + 91h], 1
        jne finished
        test byte ptr [edi + 28h], 4
        je finished

        lea edx, [esp + 1Ch]
        lea esi, [edi + 44h]
        push edx
        mov ecx, esi
        mov dword ptr [esp + 20h], 42h
        call CVectorMap::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 42h
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov esi, dword ptr [eax + 4]
        push 10h
        call Sub_Allocate
        add esp, 4
        test eax, eax
        je allocation_failed
        push ebx
        mov ecx, eax
        call CGSIPickLockedEvent::Initialize
        push eax
        mov ecx, esi
        call CGSIEntityEvents::AddEvent
        pop ebx
        pop edi
        pop esi
        add esp, 0Ch
        ret 4

allocation_failed:
        xor eax, eax
        push eax
        mov ecx, esi
        call CGSIEntityEvents::AddEvent
finished:
        pop ebx
unavailable:
        pop edi
        pop esi
        add esp, 0Ch
        ret 4
    }
}
