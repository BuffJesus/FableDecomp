#include <cstddef>

struct CCharString
{
    std::byte m_Data[0x04];
};

static_assert(sizeof(CCharString) == 0x04);

class CQuestManager;
class CGameScriptInterface
{
public:
    virtual void __thiscall SetBoastAsFailed(long boastId, const CCharString& boastName) const;
};

namespace
{
using CCharString_CopyConstructFn =
    void(__thiscall*)(CCharString* pThis, const CCharString* pOther);

using QuestManager_SetBoastThunk =
    void(__thiscall*)(CQuestManager* pThis, long boastId, CCharString boastName);

inline constexpr auto g_QuestManager =
    reinterpret_cast<CQuestManager**>(0x013B89FC);

inline constexpr auto CCharString__CCharString =
    reinterpret_cast<CCharString_CopyConstructFn>(0x0099EC30);

inline constexpr auto CQuestManager__SetBoastAsCompleted =
    reinterpret_cast<QuestManager_SetBoastThunk>(0x004AF880);
}

void __thiscall CGameScriptInterface::SetBoastAsFailed(
    long boastId,
    const CCharString& boastName) const
{
    CCharString boastNameCopy;
    CCharString__CCharString(&boastNameCopy, &boastName);
    CQuestManager__SetBoastAsCompleted(*g_QuestManager, boastId, boastNameCopy);
}