#include <cstdint>

class CCharString;
class CGameScriptInterface
{
public:
    bool __thiscall IsQuestActive(const CCharString& questName) const;
};

namespace
{
using Fn_004AF610 = bool (__thiscall*)(void* pQuestArray, const CCharString& questName);

inline constexpr auto CALL_004AF610 =
    reinterpret_cast<Fn_004AF610>(0x004AF610);

inline void* const& g_CQuestArray =
    *reinterpret_cast<void* const*>(0x013B89FC);
}

bool __thiscall CGameScriptInterface::IsQuestActive(const CCharString& questName) const
{
    return CALL_004AF610(g_CQuestArray, questName);
}