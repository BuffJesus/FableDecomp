#include <cstddef>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CQuestManager;
class CGameScriptInterface
{
public:
    void __thiscall RemoveQuestCardFromGuild(const CCharString& questCardScriptName) const;
};

extern char const DAT_0122d70e[];

extern "C" char __thiscall CCharString__NotEqual(
    const CCharString* pThis,
    const char* pText);

namespace
{
using GetAvailableQuestCardDefNameFromScriptNameFn =
    const CCharString*(__thiscall*)(CQuestManager* pThis, const CCharString* pScriptName);

using RemoveAvailableQuestCardTypeFn =
    void(__thiscall*)(CQuestManager* pThis, const CCharString* pQuestCardDefName);

inline constexpr auto g_QuestManager =
    reinterpret_cast<CQuestManager**>(0x013B89FC);

inline constexpr auto CQuestManager__GetAvailableQuestCardDefNameFromScriptName =
    reinterpret_cast<GetAvailableQuestCardDefNameFromScriptNameFn>(0x005AD670);

inline constexpr auto CQuestManager__RemoveAvailableQuestCardType =
    reinterpret_cast<RemoveAvailableQuestCardTypeFn>(0x005AEEB0);
}

void __thiscall CGameScriptInterface::RemoveQuestCardFromGuild(
    const CCharString& questCardScriptName) const
{
    const CCharString* const pQuestCardDefName =
        CQuestManager__GetAvailableQuestCardDefNameFromScriptName(
            *g_QuestManager,
            &questCardScriptName);

    CCharString questCardDefName(*pQuestCardDefName);

    if (CCharString__NotEqual(&questCardDefName, DAT_0122d70e) != 0)
    {
        CQuestManager__RemoveAvailableQuestCardType(*g_QuestManager, &questCardDefName);
    }
}