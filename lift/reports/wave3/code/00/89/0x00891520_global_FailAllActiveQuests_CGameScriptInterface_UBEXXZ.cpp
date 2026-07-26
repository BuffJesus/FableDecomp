#include <cstdint>

class CQuestManager;
class CGameScriptInterface
{
public:
    void __thiscall FailAllActiveQuests() const;
};

namespace
{
using DeactivateEventQuestsFn = void(__thiscall*)(CQuestManager*);

inline constexpr auto g_QuestManager = reinterpret_cast<CQuestManager**>(0x013B89FC);
inline constexpr DeactivateEventQuestsFn DeactivateEventQuests =
    reinterpret_cast<DeactivateEventQuestsFn>(0x004B2AD0);
}

void __thiscall CGameScriptInterface::FailAllActiveQuests() const
{
    DeactivateEventQuests(*g_QuestManager);
}