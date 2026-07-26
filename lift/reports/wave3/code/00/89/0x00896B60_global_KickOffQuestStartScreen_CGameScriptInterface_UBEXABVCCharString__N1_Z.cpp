#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCQuestCompletionUI;
class CTCHero;
class CWorld;

class CCharString
{
public:
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CWideString
{
public:
    std::uint32_t m_Value; // 0x00
};

static_assert(sizeof(CWideString) == 0x04);

enum ETCInterfaceType : std::int32_t
{
};

enum ETutorialCategory : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CWorld
{
public:
    void __thiscall AutoSaveQuestStart();
};

class CTCQuestCompletionUI
{
public:
    void __thiscall QuestCard_SetupDialog(const CCharString& questName);
    void __thiscall Activate(
        bool arg0,
        const CCharString& questName,
        bool arg2,
        const CWideString& text,
        bool arg4);
};

class CTCHero
{
public:
    bool __thiscall GiveTutorialForCategory(
        ETutorialCategory category,
        bool forceDisplay);
};

class CGameScriptInterface
{
public:
    void __thiscall KickOffQuestStartScreen(
        const CCharString& questName,
        bool showCompletionUI,
        bool addActivatedQuestName) const;
};

extern "C" CCharString* __thiscall CCharString__AssignFromWide(
    CCharString* pThis,
    const wchar_t* pText);
extern "C" void __cdecl CQuestManager__AddActivatedQuestName(const CCharString* pQuestName);

extern std::uint8_t DAT_01375741;
extern DAT_0122d70c[];
extern void* DAT_013d2838;

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceVTableOverlay
{
    std::byte m_Pad00[0x1C];
    void (__thiscall* m_RawSlot1C)(const CGameScriptInterface* pThis);  // 0x1C
    std::byte m_Pad20[0x608];
    char (__thiscall* m_RawSlot628)(const CGameScriptInterface* pThis); // 0x628
};

static_assert(offsetof(CGameScriptInterfaceVTableOverlay, m_RawSlot1C) == 0x1C);
static_assert(offsetof(CGameScriptInterfaceVTableOverlay, m_RawSlot628) == 0x628);

struct CGameScriptInterfaceOverlay
{
    const CGameScriptInterfaceVTableOverlay* m_pVTable; // 0x00
    CWorld* m_pWorld;                                   // 0x04
    std::byte m_Pad08[0x0C];
    void* m_Field14;                                    // 0x14
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pWorld) == 0x04);
static_assert(offsetof(CGameScriptInterfaceOverlay, m_Field14) == 0x14);

struct KickOffQuestStartScreen_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24;         // 0x24
    std::byte m_Pad28[0x14];
    std::uint32_t m_Flags3C;         // 0x3C
    std::byte m_Pad40[0x04];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91;          // 0x91
};

static_assert(offsetof(KickOffQuestStartScreen_TargetOverlay, m_Flags24) == 0x24);
static_assert(offsetof(KickOffQuestStartScreen_TargetOverlay, m_Flags3C) == 0x3C);
static_assert(offsetof(KickOffQuestStartScreen_TargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(KickOffQuestStartScreen_TargetOverlay, m_Flags91) == 0x91);

struct CTCQuestCompletionUIOverlay
{
    std::byte m_Pad00[0x0C];
    std::uint8_t m_Field0C; // 0x0C
    std::byte m_Pad0D[0x0F];
    std::uint8_t m_Field1C; // 0x1C
    std::byte m_Pad1D[0xA3];
    std::uint8_t m_FieldC0; // 0xC0
};

static_assert(offsetof(CTCQuestCompletionUIOverlay, m_Field0C) == 0x0C);
static_assert(offsetof(CTCQuestCompletionUIOverlay, m_Field1C) == 0x1C);
static_assert(offsetof(CTCQuestCompletionUIOverlay, m_FieldC0) == 0xC0);

struct DAT_013d2838_Overlay
{
    std::byte m_Pad00[0x05];
    std::uint8_t m_Flag05; // 0x05
};

static_assert(offsetof(DAT_013d2838_Overlay, m_Flag05) == 0x05);

union QuestStartTextTemp
{
    CCharString m_CharString;
    CWideString m_WideString;

    QuestStartTextTemp()
    {
    }

    ~QuestStartTextTemp()
    {
    }
};

static_assert(sizeof(QuestStartTextTemp) == 0x04);

extern "C" void* __thiscall Catch_0040d959(void* pArg);
extern "C" KickOffQuestStartScreen_TargetOverlay* __thiscall Sub_00487DD0(void* pArg);
}

void __thiscall CGameScriptInterface::KickOffQuestStartScreen(
    const CCharString& questName,
    bool showCompletionUI,
    bool addActivatedQuestName) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    if (!showCompletionUI)
    {
        auto* const target = Sub_00487DD0(Catch_0040d959(self->m_Field14));
        CTCQuestCompletionUI* pQuestCompletionUI = nullptr;

        if ((target->m_Flags3C & 0x10000000U) != 0)
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0xFC);
            auto* pEntry = target->m_InterfaceMap44.LowerBound(&interfaceType);

            if ((pEntry == target->m_InterfaceMap44.m_pEnd) || (0xFC < pEntry->m_Key))
            {
                pEntry = target->m_InterfaceMap44.m_pEnd;
            }

            pQuestCompletionUI =
                reinterpret_cast<CTCQuestCompletionUI*>(pEntry->m_Value);
        }

        pQuestCompletionUI->QuestCard_SetupDialog(questName);

        if (addActivatedQuestName && (DAT_01375741 != 0))
        {
            CQuestManager__AddActivatedQuestName(&questName);
            self->m_pWorld->AutoSaveQuestStart();
        }

        return;
    }

    auto* const target = Sub_00487DD0(Catch_0040d959(self->m_Field14));
    CTCQuestCompletionUI* pQuestCompletionUI = nullptr;

    if ((target->m_Flags3C & 0x10000000U) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0xFC);
        auto* pEntry = target->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == target->m_InterfaceMap44.m_pEnd) || (0xFC < pEntry->m_Key))
        {
            pEntry = target->m_InterfaceMap44.m_pEnd;
        }

        pQuestCompletionUI =
            reinterpret_cast<CTCQuestCompletionUI*>(pEntry->m_Value);
    }

    {
        QuestStartTextTemp text;
        CCharString__AssignFromWide(&text.m_CharString, DAT_0122d70c);
        pQuestCompletionUI->Activate(false, questName, true, text.m_WideString, true);
        text.m_CharString.~CCharString();
    }

    if (self->m_pVTable->m_RawSlot628(this) != '\0')
    {
        auto* pQuestCompletionUIOverlay =
            reinterpret_cast<CTCQuestCompletionUIOverlay*>(pQuestCompletionUI);

        while ((pQuestCompletionUIOverlay->m_Field0C != 0) &&
               (pQuestCompletionUIOverlay->m_Field1C == 0) &&
               (pQuestCompletionUIOverlay->m_FieldC0 == 0) &&
               (reinterpret_cast<DAT_013d2838_Overlay*>(DAT_013d2838)->m_Flag05 == 0))
        {
            self->m_pVTable->m_RawSlot1C(this);
            pQuestCompletionUIOverlay =
                reinterpret_cast<CTCQuestCompletionUIOverlay*>(pQuestCompletionUI);
        }

        if (reinterpret_cast<DAT_013d2838_Overlay*>(DAT_013d2838)->m_Flag05 != 0)
        {
            return;
        }
    }

    auto* const tutorialTarget = Sub_00487DD0(Catch_0040d959(self->m_Field14));
    if ((tutorialTarget != nullptr) &&
        ((tutorialTarget->m_Flags91 & 0x01) == 0) &&
        ((tutorialTarget->m_Flags24 & 0x200U) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x29);
        auto* pEntry = tutorialTarget->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == tutorialTarget->m_InterfaceMap44.m_pEnd) || (0x29 < pEntry->m_Key))
        {
            pEntry = tutorialTarget->m_InterfaceMap44.m_pEnd;
        }

        if (pEntry->m_Value != nullptr)
        {
            reinterpret_cast<CTCHero*>(pEntry->m_Value)->GiveTutorialForCategory(
                static_cast<ETutorialCategory>(0x1C),
                false);
            return;
        }
    }
}