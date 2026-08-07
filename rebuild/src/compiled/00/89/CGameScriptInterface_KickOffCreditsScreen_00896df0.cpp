#include <stddef.h>

class CCharString
{
public:
    void* m_Value;
    CCharString(const CCharString& other);
};

class CTCBase;
class CTCCreditsUI;

enum ETCInterfaceType
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;
    TValue m_Value;
};

typedef CKeyPair<ETCInterfaceType, CTCBase*> KickOffCreditsScreenPair;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin;
    CKeyPair<TKey, TValue>* m_pEnd;

    CKeyPair<TKey, TValue>* LowerBound(const TKey* pKey);
};

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

typedef CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >
    KickOffCreditsScreenInterfaceMap;

struct CGameScriptInterfaceVTableOverlay
{
    unsigned char m_Pad00[0x1C];
    void (__fastcall* m_RawSlot1C)(CGameScriptInterface* pThis);
};

struct CGameScriptInterfaceOverlay
{
    const CGameScriptInterfaceVTableOverlay* m_pVTable;
    unsigned char m_Pad04[0x10];
    void* m_Field14;
};

struct KickOffCreditsScreenResolvedOverlay
{
    unsigned char m_Pad00[0x40];
    unsigned char m_Flags40;
    unsigned char m_Pad41[0x03];
    KickOffCreditsScreenInterfaceMap m_InterfaceMap44;
};

typedef void* (__fastcall* Helper00449970Fn)(void*);
typedef KickOffCreditsScreenResolvedOverlay* (__fastcall* Helper00487DC0Fn)(void*);

extern "C" void* __fastcall Helper00449970(void* value);
extern "C" KickOffCreditsScreenResolvedOverlay* __fastcall Helper00487DC0(void* value);

void CGameScriptInterface::KickOffCreditsScreen(
    CCharString& screenName)
{
    const CGameScriptInterfaceOverlay* self =
        (const CGameScriptInterfaceOverlay*)this;
    CTCCreditsUI* creditsUI = 0;
    KickOffCreditsScreenResolvedOverlay* resolved =
        Helper00487DC0(Helper00449970(self->m_Field14));

    if ((resolved->m_Flags40 & 0x02) != 0)
    {
        ETCInterfaceType interfaceType =
            (ETCInterfaceType)0x101;
        KickOffCreditsScreenInterfaceMap* map =
            (KickOffCreditsScreenInterfaceMap*)&resolved->m_InterfaceMap44;
        KickOffCreditsScreenPair* entry =
            map->LowerBound(&interfaceType);
        if (entry == map->m_pEnd ||
            (ETCInterfaceType)0x101 < entry->m_Key)
            entry = map->m_pEnd;
        creditsUI = (CTCCreditsUI*)entry->m_Value;
    }

    creditsUI->Activate(CCharString(screenName));
    self->m_pVTable->m_RawSlot1C((CGameScriptInterface*)this);
}
