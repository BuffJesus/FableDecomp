#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall UpdateCounter(long questId, long value1, long value2);
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
    virtual void __thiscall UpdateQuestInfoCounter(long questId, long value1, long value2);
};

void __thiscall CGameScriptInterface::UpdateQuestInfoCounter(long questId, long value1, long value2)
{
    DAT_013b8790->m_pDrawQuestInfo->UpdateCounter(questId, value1, value2);
}
