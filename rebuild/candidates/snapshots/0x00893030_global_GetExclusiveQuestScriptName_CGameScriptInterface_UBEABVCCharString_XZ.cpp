#include <cstdint>

class CCharString;
class CQuestManager;
class CGameScriptInterface
{
public:
    const CCharString& __thiscall GetExclusiveQuestScriptName() const;
};

class CQuestManager
{
public:
    const CCharString& __thiscall GetActiveExclusiveQuestName();
};

namespace
{
inline constexpr auto g_QuestManager =
    reinterpret_cast<CQuestManager**>(0x013B89FC);
}

const CCharString& __thiscall CGameScriptInterface::GetExclusiveQuestScriptName() const
{
    return (*g_QuestManager)->GetActiveExclusiveQuestName();
}