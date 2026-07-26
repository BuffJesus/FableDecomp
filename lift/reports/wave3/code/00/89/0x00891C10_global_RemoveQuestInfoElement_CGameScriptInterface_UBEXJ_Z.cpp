#include <cstddef>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall RemoveElement(long elementId);
};
}

class CGameScriptInterface
{
public:
    void __thiscall RemoveQuestInfoElement(long elementId) const;
};

struct QuestInfoGuiOwnerOverlay
{
    std::byte pad_0000[0x1C8];
    NPlayerGui::CDrawQuestInfo* pDrawQuestInfo;
};

static_assert(offsetof(QuestInfoGuiOwnerOverlay, pDrawQuestInfo) == 0x1C8);

extern QuestInfoGuiOwnerOverlay* DAT_013b8790;

void __thiscall CGameScriptInterface::RemoveQuestInfoElement(long elementId) const
{
    DAT_013b8790->pDrawQuestInfo->RemoveElement(elementId);
}