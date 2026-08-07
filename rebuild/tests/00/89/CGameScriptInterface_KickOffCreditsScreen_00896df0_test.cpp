#include <stdio.h>

class CCharString
{
public:
    const char* text;
    CCharString() : text("credits_screen") {}
    CCharString(const CCharString& other) : text(other.text) {}
};

class CTCBase;
class CTCCreditsUI;
enum ETCInterfaceType { };

template <typename TKey, typename TValue> class CKeyPairCompareLess;
template <typename TKey, typename TValue> struct CKeyPair
{
    TKey m_Key;
    TValue m_Value;
};

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin;
    CKeyPair<TKey, TValue>* m_pEnd;
    CKeyPair<TKey, TValue>* LowerBound(const TKey* key);
};

typedef CKeyPair<ETCInterfaceType, CTCBase*> Pair;
typedef CVectorMap<ETCInterfaceType, CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > InterfaceMap;

class CTCCreditsUI
{
public:
    void __fastcall Activate(CCharString screenName);
};

class CGameScriptInterface
{
public:
    void KickOffCreditsScreen(CCharString& screenName);
};

struct VTable { unsigned char pad[0x1c];
    void (__fastcall* slot1c)(CGameScriptInterface*); };
struct Self { VTable* vtable; unsigned char pad04[0x10]; void* field14; };
struct Resolved { unsigned char pad[0x40]; unsigned char flags;
    unsigned char pad41[3]; InterfaceMap map; };

static void* g_helperInput;
static void* g_helperOutput;
static void* g_creditsInput;
static int g_activateCalls;
static int g_slotCalls;
static const char* g_name;
static InterfaceMap* g_map;
static Pair* g_lowerBoundResult;
static ETCInterfaceType g_lowerBoundKey;

extern "C" void* __fastcall Helper00449970(void* value)
{ g_helperInput = value; return g_helperOutput; }
extern "C" Resolved* __fastcall Helper00487DC0(void* value)
{ g_creditsInput = value; return (Resolved*)value; }

Pair* InterfaceMap::LowerBound(const ETCInterfaceType* key)
{ g_map = this; g_lowerBoundKey = *key; return g_lowerBoundResult; }
void __fastcall CTCCreditsUI::Activate(CCharString screenName)
{ ++g_activateCalls; g_name = screenName.text; }
static void __fastcall Slot(CGameScriptInterface*) { ++g_slotCalls; }

void CGameScriptInterface::KickOffCreditsScreen(
    CCharString& screenName)
{
    const Self* self = (const Self*)this;
    CTCCreditsUI* creditsUI = 0;
    Resolved* resolved = Helper00487DC0(Helper00449970(self->field14));
    if ((resolved->flags & 0x02) != 0)
    {
        ETCInterfaceType interfaceType = (ETCInterfaceType)0x101;
        InterfaceMap* map = &resolved->map;
        Pair* entry = map->LowerBound(&interfaceType);
        if (entry == map->m_pEnd ||
            (ETCInterfaceType)0x101 < entry->m_Key)
            entry = map->m_pEnd;
        creditsUI = (CTCCreditsUI*)entry->m_Value;
    }
    creditsUI->Activate(CCharString(screenName));
    self->vtable->slot1c((CGameScriptInterface*)this);
}

static void Reset()
{
    g_helperInput = 0; g_helperOutput = 0; g_creditsInput = 0;
    g_activateCalls = 0; g_slotCalls = 0; g_name = 0;
    g_map = 0; g_lowerBoundResult = 0; g_lowerBoundKey = (ETCInterfaceType)0;
}

int main()
{
    Self self;
    VTable vtable;
    Resolved resolved;
    Pair entry;
    Pair end;
    CTCCreditsUI credits;
    CCharString name;
    vtable.slot1c = Slot;
    self.vtable = &vtable;
    self.field14 = (void*)0x12340000;
    resolved.flags = 0x02;
    resolved.map.m_pEnd = &end;
    entry.m_Key = (ETCInterfaceType)0x101;
    entry.m_Value = (CTCBase*)&credits;
    end.m_Key = (ETCInterfaceType)0;
    end.m_Value = (CTCBase*)&credits;
    name.text = "credits_screen";

    Reset();
    g_helperOutput = &resolved;
    g_lowerBoundResult = &entry;
    ((CGameScriptInterface*)&self)->KickOffCreditsScreen(name);
    if (g_helperInput != self.field14 || g_creditsInput != &resolved ||
        g_map != &resolved.map || g_lowerBoundKey != (ETCInterfaceType)0x101 ||
        g_activateCalls != 1 || g_slotCalls != 1 || g_name != name.text)
    { printf("normal path failed\n"); return 1; }

    Reset();
    resolved.flags = 0;
    g_helperOutput = &resolved;
    g_lowerBoundResult = &entry;
    ((CGameScriptInterface*)&self)->KickOffCreditsScreen(name);
    if (g_activateCalls != 1 || g_slotCalls != 1 || g_map != 0)
    { printf("flag bypass failed\n"); return 1; }

    printf("FSE2_00896df0_TEST PASS\n");
    return 0;
}
