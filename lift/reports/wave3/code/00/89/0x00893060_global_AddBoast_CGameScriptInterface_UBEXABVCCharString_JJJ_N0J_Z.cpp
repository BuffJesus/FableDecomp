#include <cstddef>
#include <cstdint>

struct CCharStringData
{
    std::byte m_Pad00[0x04];
    std::int32_t m_Length; // 0x04
};
static_assert(offsetof(CCharStringData, m_Length) == 0x04);

class CCharString
{
public:
    CCharStringData* m_pData; // 0x00

    CCharString(const CCharString& other);
    CCharString& __thiscall operator=(const CCharString& rhs);
    ~CCharString();
};
static_assert(offsetof(CCharString, m_pData) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CQuestManager
{
public:
    void __thiscall AddBoast(
        const CCharString& resolvedQuestName,
        const CCharString& boastName,
        long value1,
        long value2,
        long value3,
        bool enabled,
        long value4);
};

class CGameScriptInterface
{
public:
    virtual void __thiscall AddBoast(
        const CCharString& boastName,
        long value1,
        long value2,
        long value3,
        bool enabled,
        const CCharString& questName,
        long value4) const;
};

extern CQuestManager* DAT_013b89fc;

namespace
{
struct BoastEntryOverlay
{
    std::byte m_Pad00[0x30];
    CCharString m_QuestName; // 0x30
};
static_assert(offsetof(BoastEntryOverlay, m_QuestName) == 0x30);

struct BoastListNodeOverlay
{
    BoastListNodeOverlay* m_pNext; // 0x00
    BoastListNodeOverlay* m_pPrev; // 0x04
    BoastEntryOverlay* m_pEntry;   // 0x08
};
static_assert(offsetof(BoastListNodeOverlay, m_pEntry) == 0x08);

struct QuestManagerBoastOverlay
{
    std::byte m_Pad00[0x38];
    BoastListNodeOverlay* m_pBoastListHead; // 0x38
    std::byte m_Pad3C[0x4C];
    BoastEntryOverlay* m_pCurrentBoastEntry; // 0x88
};
static_assert(offsetof(QuestManagerBoastOverlay, m_pBoastListHead) == 0x38);
static_assert(offsetof(QuestManagerBoastOverlay, m_pCurrentBoastEntry) == 0x88);
}

void __thiscall CGameScriptInterface::AddBoast(
    const CCharString& boastName,
    long value1,
    long value2,
    long value3,
    bool enabled,
    const CCharString& questName,
    long value4) const
{
    const auto* const managerOverlay =
        reinterpret_cast<const QuestManagerBoastOverlay*>(DAT_013b89fc);

    BoastEntryOverlay* boastEntry = managerOverlay->m_pCurrentBoastEntry;
    if (boastEntry == nullptr)
    {
        BoastListNodeOverlay* const head = managerOverlay->m_pBoastListHead;
        if (head->m_pNext != head)
        {
            boastEntry = head->m_pPrev->m_pEntry;
        }
    }

    CCharString resolvedQuestName(questName);
    if (((resolvedQuestName.m_pData == nullptr) ||
         (resolvedQuestName.m_pData->m_Length == 0)) &&
        (boastEntry != nullptr))
    {
        resolvedQuestName = boastEntry->m_QuestName;
    }

    DAT_013b89fc->AddBoast(
        resolvedQuestName,
        boastName,
        value1,
        value2,
        value3,
        enabled,
        value4);
}
