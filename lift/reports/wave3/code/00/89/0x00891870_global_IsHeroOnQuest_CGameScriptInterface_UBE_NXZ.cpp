#include <cstdint>

class CQuestManager;
class CGameScriptInterface
{
public:
    bool __thiscall IsHeroOnQuest() const;
};

namespace
{
using IsBetweenQuestStartAndFinishScreensIgnoringVignettesFn =
    bool(__thiscall*)(CQuestManager*);

inline constexpr auto g_QuestManager =
    reinterpret_cast<CQuestManager**>(0x013B89FC);
inline constexpr IsBetweenQuestStartAndFinishScreensIgnoringVignettesFn
    IsBetweenQuestStartAndFinishScreensIgnoringVignettes =
        reinterpret_cast<IsBetweenQuestStartAndFinishScreensIgnoringVignettesFn>(
            0x004AE110);
}

bool __thiscall CGameScriptInterface::IsHeroOnQuest() const
{
    return IsBetweenQuestStartAndFinishScreensIgnoringVignettes(*g_QuestManager);
}