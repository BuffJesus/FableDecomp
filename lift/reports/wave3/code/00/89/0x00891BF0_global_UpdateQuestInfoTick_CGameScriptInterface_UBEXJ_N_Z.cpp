#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall UpdateTick(long questId, bool enabled);
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
    virtual void __thiscall UpdateQuestInfoTick(long questId, bool enabled);
};

void __thiscall CGameScriptInterface::UpdateQuestInfoTick(long questId, bool enabled)
{
    DAT_013b8790->m_pDrawQuestInfo->UpdateTick(questId, enabled);
}