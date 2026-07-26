#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall UpdateCounterList(long questId, long value1, long value2);
};
}

struct QuestInfoBarOwnerOverlay
{
    std::byte m_Pad00[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_pDrawQuestInfo; // 0x1C8
};
static_assert(offsetof(QuestInfoBarOwnerOverlay, m_pDrawQuestInfo) == 0x1C8);

extern QuestInfoBarOwnerOverlay* DAT_013b8790;

class CGameScriptInterface
{
public:
    virtual void __thiscall UpdateQuestInfoCounterList(long questId, long value1, long value2);
};

void __thiscall CGameScriptInterface::UpdateQuestInfoCounterList(
    long questId,
    long value1,
    long value2)
{
    DAT_013b8790->m_pDrawQuestInfo->UpdateCounterList(questId, value1, value2);
}
