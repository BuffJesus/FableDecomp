#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall UpdateBar(long questId, float value1, float value2, float value3);
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
    virtual void __thiscall UpdateQuestInfoBar(
        long questId,
        float value1,
        float value2,
        float value3) const;
};

void __thiscall CGameScriptInterface::UpdateQuestInfoBar(
    long questId,
    float value1,
    float value2,
    float value3) const
{
    DAT_013b8790->m_pDrawQuestInfo->UpdateBar(questId, value1, value2, value3);
}