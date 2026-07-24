#include <cstddef>
#include <cstdint>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall DisplayQuestInfo(bool display);
};
}

class CGameScriptInterface
{
public:
    void __thiscall DisplayQuestInfo(bool display) const;
};

extern "C" std::uint32_t DAT_013b8790;

namespace
{
struct DisplayQuestInfo_GlobalOverlay
{
    std::byte m_Pad00[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_pDrawQuestInfo; // 0x1C8
};

static_assert(offsetof(DisplayQuestInfo_GlobalOverlay, m_pDrawQuestInfo) == 0x1C8);
}

void __thiscall CGameScriptInterface::DisplayQuestInfo(bool /*display*/) const
{
    NPlayerGui::CDrawQuestInfo::DisplayQuestInfo(
        reinterpret_cast<DisplayQuestInfo_GlobalOverlay*>(DAT_013b8790)->m_pDrawQuestInfo,
        true);
}