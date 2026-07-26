#include <cstddef>
#include <cstdint>

class CBaseIntelligentPointer;
class CBaseObject;
class CCharString;
class CMessageEvent;
class CMessageEventManager;
class CTCBase;
class CTCCameraPointScriptedSpline;
class CWorld;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CBaseIntelligentPointer
{
public:
    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

struct CMessageEventFilter_HasExtraData
{
    std::int32_t m_Type;

    bool __thiscall operator()(CMessageEvent* pMessageEvent);
};
static_assert(sizeof(CMessageEventFilter_HasExtraData) == 0x04);

class CMessageEventManager
{
public:
    void __thiscall RemoveMessage(const CMessageEvent* pMessage);
};

class CTCCameraPointScriptedSpline
{
public:
    bool __thiscall HasMessage(const CCharString* pMessage);
};

class CGameScriptInterface
{
public:
    virtual void __thiscall WaitForCameraMessage(const CCharString& message) const;
};

extern void* DAT_013d2838;

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};
static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);

struct CWorldOverlay
{
    std::byte m_Pad00[0x60];
    CMessageEventManager* m_pMessageEventManager; // 0x60
};
static_assert(offsetof(CWorldOverlay, m_pMessageEventManager) == 0x60);

struct MessageEventManagerOverlay
{
    std::byte m_Pad00[0x04];
    void* m_pListHead; // 0x04
};
static_assert(offsetof(MessageEventManagerOverlay, m_pListHead) == 0x04);

struct MessageEventNodeOverlay
{
    MessageEventNodeOverlay* m_pNext; // 0x00
    MessageEventNodeOverlay* m_pPrev; // 0x04
    std::byte m_MessageEventStorage[0x48]; // 0x08
};
static_assert(offsetof(MessageEventNodeOverlay, m_pNext) == 0x00);
static_assert(offsetof(MessageEventNodeOverlay, m_pPrev) == 0x04);
static_assert(offsetof(MessageEventNodeOverlay, m_MessageEventStorage) == 0x08);

struct MessageEventOverlay
{
    std::byte m_Pad00[0x48];
    CBaseIntelligentPointer m_Object; // 0x48
};
static_assert(offsetof(MessageEventOverlay, m_Object) == 0x48);

struct TCInterfaceEntryOverlay
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_pValue; // 0x04
};
static_assert(offsetof(TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct BaseObjectOverlay
{
    std::byte m_Pad00[0x24];
    std::int32_t m_Field24; // 0x24
    std::byte m_Pad28[0x1C];
    TCInterfaceMap m_TCInterfaces; // 0x44
    TCInterfaceEntryOverlay* m_pTCInterfacesEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(BaseObjectOverlay, m_Field24) == 0x24);
static_assert(offsetof(BaseObjectOverlay, m_TCInterfaces) == 0x44);
static_assert(offsetof(BaseObjectOverlay, m_pTCInterfacesEnd) == 0x48);
static_assert(offsetof(BaseObjectOverlay, m_Flags91) == 0x91);

struct Global013D2838VftOverlay
{
    std::byte m_Pad00[0x08];
    void(__thiscall* m_Slot08)(void* pThis); // 0x08
};
static_assert(offsetof(Global013D2838VftOverlay, m_Slot08) == 0x08);

struct Global013D2838Overlay
{
    Global013D2838VftOverlay* m_pVft; // 0x00
    std::byte m_Pad04[0x01];
    std::uint8_t m_Flag05; // 0x05
};
static_assert(offsetof(Global013D2838Overlay, m_pVft) == 0x00);
static_assert(offsetof(Global013D2838Overlay, m_Flag05) == 0x05);

struct Local4Overlay
{
    std::uint32_t m_Raw;
};
static_assert(sizeof(Local4Overlay) == 0x04);
}

void __thiscall CGameScriptInterface::WaitForCameraMessage(const CCharString& message) const
{
    const auto* const pSelf = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    do
    {
        Local4Overlay local_4{};
        local_4.m_Raw = 0x2B;

        auto* const pWorld = reinterpret_cast<CWorldOverlay*>(pSelf->m_pWorld);
        auto* const iVar2 =
            reinterpret_cast<MessageEventManagerOverlay*>(pWorld->m_pMessageEventManager);
        auto* piVar5 = reinterpret_cast<MessageEventNodeOverlay*>(iVar2->m_pListHead);
        auto* piVar6 = piVar5->m_pNext;

        if (piVar6 != piVar5)
        {
            do
            {
                auto* const pMessageEvent =
                    reinterpret_cast<CMessageEvent*>(&piVar6->m_MessageEventStorage[0]);

                if (reinterpret_cast<CMessageEventFilter_HasExtraData*>(&local_4)->operator()(
                        pMessageEvent))
                {
                    if (pMessageEvent != nullptr)
                    {
                        auto* const pIntelligentPointer =
                            reinterpret_cast<CBaseIntelligentPointer*>(
                                reinterpret_cast<std::byte*>(piVar6) + 0x50);
                        CBaseObject* const pObject =
                            CBaseIntelligentPointer::GetPItem(pIntelligentPointer);

                        if (pObject != nullptr)
                        {
                            local_4.m_Raw = static_cast<std::uint32_t>(
                                reinterpret_cast<std::uintptr_t>(
                                    CBaseIntelligentPointer::GetPItem(pIntelligentPointer)));

                            auto* const pObjectOverlay = reinterpret_cast<BaseObjectOverlay*>(
                                CBaseIntelligentPointer::GetPItem(pIntelligentPointer));

                            if ((pObjectOverlay != nullptr) &&
                                ((pObjectOverlay->m_Flags91 & 0x01) == 0) &&
                                (pObjectOverlay->m_Field24 < 0))
                            {
                                local_4.m_Raw = 0x3F;

                                auto* pInterfaceEntry = reinterpret_cast<TCInterfaceEntryOverlay*>(
                                    TCInterfaceMap::LowerBound(
                                        &pObjectOverlay->m_TCInterfaces,
                                        reinterpret_cast<const ETCInterfaceType*>(&local_4)));

                                if ((pInterfaceEntry == pObjectOverlay->m_pTCInterfacesEnd) ||
                                    (0x3F < static_cast<std::int32_t>(pInterfaceEntry->m_Key)))
                                {
                                    pInterfaceEntry = pObjectOverlay->m_pTCInterfacesEnd;
                                }

                                if (reinterpret_cast<CTCCameraPointScriptedSpline*>(
                                        pInterfaceEntry->m_pValue)
                                        ->HasMessage(&message))
                                {
                                    reinterpret_cast<CWorldOverlay*>(pSelf->m_pWorld)
                                        ->m_pMessageEventManager
                                        ->RemoveMessage(pMessageEvent);
                                    return;
                                }
                            }
                        }
                    }

                    break;
                }

                piVar6 = piVar6->m_pNext;
            } while (
                piVar6 != reinterpret_cast<MessageEventNodeOverlay*>(iVar2->m_pListHead));
        }

        auto* const pGlobal = reinterpret_cast<Global013D2838Overlay*>(DAT_013d2838);
        pGlobal->m_pVft->m_Slot08(DAT_013d2838);

        if (reinterpret_cast<Global013D2838Overlay*>(DAT_013d2838)->m_Flag05 != 0)
        {
            return;
        }
    } while (true);
}
