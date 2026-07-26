#include <cstddef>

class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall UpdateMiniGame(float value, int a3, int a4);
};
}

namespace
{
extern std::byte* DAT_013b8790;

struct UpdateMiniGameInfoBar_GlobalOverlay
{
    std::byte m_Pad00[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_pDrawQuestInfo; // 0x1C8
};
static_assert(offsetof(UpdateMiniGameInfoBar_GlobalOverlay, m_pDrawQuestInfo) == 0x1C8);
}

class CGameScriptInterface
{
public:
    void __thiscall UpdateMiniGameInfoBar(float value) const;
};

void __thiscall CGameScriptInterface::UpdateMiniGameInfoBar(float value) const
{
    reinterpret_cast<UpdateMiniGameInfoBar_GlobalOverlay*>(DAT_013b8790)
        ->m_pDrawQuestInfo
        ->UpdateMiniGame(value, 0, 0);
}