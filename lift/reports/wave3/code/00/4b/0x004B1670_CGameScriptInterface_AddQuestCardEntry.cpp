#include <cstddef>
#include <cstdint>

class CCharString
{
public:
    CCharString();
    CCharString& __thiscall operator=(const CCharString& other);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall AddQuestCardEntry(
        CCharString* questName,
        CCharString* questDescription,
        std::uint8_t addToFront);
};

namespace
{
struct QuestCardListNodeOverlay
{
    QuestCardListNodeOverlay* m_pNext; // 0x00
};

static_assert(offsetof(QuestCardListNodeOverlay, m_pNext) == 0x00);
static_assert(sizeof(QuestCardListNodeOverlay) == 0x04);

struct QuestCardListIteratorOverlay
{
    QuestCardListNodeOverlay* m_pNode; // 0x00
};

static_assert(offsetof(QuestCardListIteratorOverlay, m_pNode) == 0x00);
static_assert(sizeof(QuestCardListIteratorOverlay) == 0x04);

struct QuestCardListOverlay
{
    QuestCardListNodeOverlay* m_pSentinel; // 0x00
};

static_assert(offsetof(QuestCardListOverlay, m_pSentinel) == 0x00);
static_assert(sizeof(QuestCardListOverlay) == 0x04);

struct QuestCardEntryValueOverlay
{
    CCharString m_QuestName;        // 0x00
    CCharString m_QuestDescription; // 0x04
    std::uint8_t m_AddToFront;      // 0x08
};

static_assert(offsetof(QuestCardEntryValueOverlay, m_QuestName) == 0x00);
static_assert(offsetof(QuestCardEntryValueOverlay, m_QuestDescription) == 0x04);
static_assert(offsetof(QuestCardEntryValueOverlay, m_AddToFront) == 0x08);

struct CGameScriptInterface_AddQuestCardEntry_Overlay
{
    std::byte m_Pad00[0x58];
    QuestCardListOverlay m_QuestCardList; // 0x58
    std::byte m_Pad5C[0x8F - 0x5C];
    std::uint8_t m_QuestCardDirty; // 0x8F
};

static_assert(offsetof(CGameScriptInterface_AddQuestCardEntry_Overlay, m_QuestCardList) == 0x58);
static_assert(offsetof(CGameScriptInterface_AddQuestCardEntry_Overlay, m_QuestCardDirty) == 0x8F);

using FindQuestCardEntryRawFn = void(__fastcall*)(
    QuestCardListIteratorOverlay* pResult,
    std::uint8_t* pAddToFrontParamHome,
    QuestCardListIteratorOverlay current,
    const QuestCardListIteratorOverlay* pSentinel,
    const CCharString* pQuestName);

using InsertQuestCardEntryRawFn = void(__thiscall*)(
    QuestCardListOverlay* pList,
    const QuestCardEntryValueOverlay* pValue,
    QuestCardListIteratorOverlay position);

inline constexpr auto std___Find__QuestCardListIterator_CCharString =
    reinterpret_cast<FindQuestCardEntryRawFn>(0x004B91F0);

inline constexpr auto QuestCardList__InsertRaw =
    reinterpret_cast<InsertQuestCardEntryRawFn>(0x004B99C0);
}

void __thiscall CGameScriptInterface::AddQuestCardEntry(
    CCharString* questName,
    CCharString* questDescription,
    std::uint8_t addToFront)
{
    auto* const self =
        reinterpret_cast<CGameScriptInterface_AddQuestCardEntry_Overlay*>(this);

    CCharString* const originalQuestName = questName;
    QuestCardListNodeOverlay* const pSentinelNode = self->m_QuestCardList.m_pSentinel;
    QuestCardListIteratorOverlay sentinelIterator;
    sentinelIterator.m_pNode = pSentinelNode;

    std___Find__QuestCardListIterator_CCharString(
        reinterpret_cast<QuestCardListIteratorOverlay*>(&questName),
        &addToFront,
        QuestCardListIteratorOverlay{pSentinelNode->m_pNext},
        &sentinelIterator,
        originalQuestName);

    if (questName == reinterpret_cast<CCharString*>(pSentinelNode))
    {
        QuestCardEntryValueOverlay value;
        std::int32_t lifetimeGuard;

        value.m_QuestName = *originalQuestName;
        value.m_QuestDescription = *questDescription;
        value.m_AddToFront = addToFront;
        lifetimeGuard = -1;

        QuestCardList__InsertRaw(
            &self->m_QuestCardList,
            &value,
            QuestCardListIteratorOverlay{self->m_QuestCardList.m_pSentinel});

        self->m_QuestCardDirty = 1;
        (void)lifetimeGuard;
    }
}