#include <cstddef>
#include <cstdint>

class CBaseIntelligentPointer;
class CBaseObject;
class CCharString;
class CMessageEvent;
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
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

class CBaseIntelligentPointer
{
public:
    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

struct CMessageEventFilter_IsOfType
{
    long Type;
};

static_assert(sizeof(CMessageEventFilter_IsOfType) == 0x04);

class CMessageEventManager
{
public:
    template <class TFilter>
    CMessageEvent* __thiscall FindMostRecentMessage(TFilter* pFilter);
};

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall GetElementByIndex(long index);
};
}

class CTCCameraPointScriptedSpline
{
public:
    bool __thiscall HasMessage(const CCharString* pMessage);
};

class CGameScriptInterface
{
public:
    virtual bool __thiscall CheckForCameraMessage(const CCharString& message) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_CheckForCameraMessage_SelfOverlay
{
    std::byte pad_00[0x04];
    CWorld* pWorld; // 0x04
};

static_assert(
    offsetof(CGameScriptInterface_CheckForCameraMessage_SelfOverlay, pWorld) == 0x04);

struct CWorldMessageEventManagerOverlay
{
    std::byte pad_00[0x60];
    CMessageEventManager* pMessageEventManager; // 0x60
};

static_assert(
    offsetof(CWorldMessageEventManagerOverlay, pMessageEventManager) == 0x60);

struct CGameScriptInterface_CheckForCameraMessage_MessageEventOverlay
{
    std::byte pad_00[0x48];
    CBaseIntelligentPointer pObject; // 0x48
};

static_assert(
    offsetof(CGameScriptInterface_CheckForCameraMessage_MessageEventOverlay, pObject) == 0x48);

struct CGameScriptInterface_CheckForCameraMessage_TCInterfaceEntryOverlay
{
    std::int32_t Key; // 0x00
    CTCBase* pValue;  // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_CheckForCameraMessage_TCInterfaceEntryOverlay,
        pValue) == 0x04);

struct CGameScriptInterface_CheckForCameraMessage_ObjectOverlay
{
    std::byte pad_00[0x24];
    std::int32_t Field24; // 0x24
    std::byte pad_28[0x1C];
    std::byte TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_CheckForCameraMessage_TCInterfaceEntryOverlay* pTCInterfaceMapEnd; // 0x48
    std::byte pad_4C[0x45];
    std::uint8_t Flags91; // 0x91
};

static_assert(
    offsetof(CGameScriptInterface_CheckForCameraMessage_ObjectOverlay, Field24) == 0x24);
static_assert(
    offsetof(
        CGameScriptInterface_CheckForCameraMessage_ObjectOverlay,
        TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_CheckForCameraMessage_ObjectOverlay,
        pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_CheckForCameraMessage_ObjectOverlay, Flags91) == 0x91);
}

bool __thiscall CGameScriptInterface::CheckForCameraMessage(
    const CCharString& message) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_CheckForCameraMessage_SelfOverlay*>(this);
    const auto* const world =
        reinterpret_cast<const CWorldMessageEventManagerOverlay*>(self->pWorld);

    CMessageEventFilter_IsOfType isOfTypeFilter{};
    isOfTypeFilter.Type = 0x2B;

    CMessageEvent* const pMessageEvent =
        world->pMessageEventManager->FindMostRecentMessage(&isOfTypeFilter);
    if (pMessageEvent != nullptr)
    {
        auto* const pMessageEventOverlay =
            reinterpret_cast<CGameScriptInterface_CheckForCameraMessage_MessageEventOverlay*>(
                pMessageEvent);
        auto* const pObject =
            CBaseIntelligentPointer::GetPItem(&pMessageEventOverlay->pObject);

        if (pObject != nullptr)
        {
            CBaseObject* const localObject =
                CBaseIntelligentPointer::GetPItem(&pMessageEventOverlay->pObject);
            (void)localObject;

            auto* const pObjectOverlay =
                reinterpret_cast<CGameScriptInterface_CheckForCameraMessage_ObjectOverlay*>(
                    CBaseIntelligentPointer::GetPItem(&pMessageEventOverlay->pObject));

            if ((pObjectOverlay != nullptr) &&
                ((pObjectOverlay->Flags91 & 0x01) == 0) &&
                (pObjectOverlay->Field24 < 0))
            {
                ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x3F);

                auto* pEntry =
                    reinterpret_cast<
                        CGameScriptInterface_CheckForCameraMessage_TCInterfaceEntryOverlay*>(
                        TCInterfaceMap::LowerBound(
                            reinterpret_cast<TCInterfaceMap*>(
                                pObjectOverlay->TCInterfaceMapStorage),
                            &interfaceType));

                if ((pEntry == pObjectOverlay->pTCInterfaceMapEnd) || (0x3F < pEntry->Key))
                {
                    pEntry = pObjectOverlay->pTCInterfaceMapEnd;
                }

                const bool hasMessage =
                    reinterpret_cast<CTCCameraPointScriptedSpline*>(pEntry->pValue)
                        ->HasMessage(&message);
                if (hasMessage)
                {
                    reinterpret_cast<NPlayerGui::CDrawQuestInfo*>(
                        world->pMessageEventManager)
                        ->GetElementByIndex(
                            static_cast<long>(reinterpret_cast<std::uintptr_t>(pMessageEvent)));
                    return true;
                }
            }
        }
    }

    return false;
}