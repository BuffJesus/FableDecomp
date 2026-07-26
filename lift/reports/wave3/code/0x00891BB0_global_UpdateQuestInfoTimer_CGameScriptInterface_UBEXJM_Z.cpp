#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall UpdateTimer(long questId, float timeValue);
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
    virtual void __thiscall UpdateQuestInfoTimer(long questId, float timeValue);
};

void __thiscall CGameScriptInterface::UpdateQuestInfoTimer(long questId, float timeValue)
{
    DAT_013b8790->m_pDrawQuestInfo->UpdateTimer(questId, timeValue);
}