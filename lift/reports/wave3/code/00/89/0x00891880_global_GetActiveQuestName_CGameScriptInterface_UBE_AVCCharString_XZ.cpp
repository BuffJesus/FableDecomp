#include <cstddef>

class CCharString;
class CGameScriptInterface;

struct ActiveQuestOverlay
{
    std::byte gap_00[0x30];
    CCharString m_Name; // 0x30
};

static_assert(offsetof(ActiveQuestOverlay, m_Name) == 0x30);

struct GameGlobalsOverlay
{
    std::byte gap_00[0x88];
    ActiveQuestOverlay* m_ActiveQuest; // 0x88
};

static_assert(offsetof(GameGlobalsOverlay, m_ActiveQuest) == 0x88);

extern GameGlobalsOverlay* DAT_013b89fc;

CCharString CGameScriptInterface::GetActiveQuestName() const
{
    if (DAT_013b89fc->m_ActiveQuest != nullptr)
    {
        return CCharString(DAT_013b89fc->m_ActiveQuest->m_Name);
    }

    return CCharString("", -1);
}