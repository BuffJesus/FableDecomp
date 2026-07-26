#include <cstdint>

class CEngine
{
public:
    void __thiscall ChangeCameraAngle();
};

class CGameScriptInterface
{
public:
    void __thiscall RemoveAllAvailableQuestCardsFromGuild() const;
};

namespace
{
inline constexpr auto g_Engine = reinterpret_cast<CEngine**>(0x013B89FC);
}

void __thiscall CGameScriptInterface::RemoveAllAvailableQuestCardsFromGuild() const
{
    (*g_Engine)->ChangeCameraAngle();
}