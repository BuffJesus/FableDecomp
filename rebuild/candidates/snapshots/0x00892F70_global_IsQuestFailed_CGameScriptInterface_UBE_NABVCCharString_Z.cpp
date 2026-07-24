#include <cstdint>

class CCharString;
class CGameScriptInterface
{
public:
    bool __thiscall IsQuestFailed(const CCharString& questName) const;
};

namespace
{
using Fn_004AF280 = bool (__thiscall*)(void* pQuestArray, const CCharString& questName);

inline constexpr auto CALL_004AF280 =
    reinterpret_cast<Fn_004AF280>(0x004AF280);

inline void* const& g_CQuestArray =
    *reinterpret_cast<void* const*>(0x013B89FC);
}

bool __thiscall CGameScriptInterface::IsQuestFailed(const CCharString& questName) const
{
    return CALL_004AF280(g_CQuestArray, questName);
}