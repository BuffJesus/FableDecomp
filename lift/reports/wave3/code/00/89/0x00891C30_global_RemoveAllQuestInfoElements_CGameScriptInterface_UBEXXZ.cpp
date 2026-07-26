#include <cstddef>
#include <cstdint>

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall Clear();
};
}

class CGameScriptInterface
{
public:
    void __thiscall RemoveAllQuestInfoElements() const;
};

extern "C" std::uint32_t DAT_013b8790;

namespace
{
struct RemoveAllQuestInfoElements_GlobalOverlay
{
    std::byte m_Pad00[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_pDrawQuestInfo; // 0x1C8
};

static_assert(
    offsetof(RemoveAllQuestInfoElements_GlobalOverlay, m_pDrawQuestInfo) == 0x1C8);
}

void __thiscall CGameScriptInterface::RemoveAllQuestInfoElements() const
{
    NPlayerGui::CDrawQuestInfo::Clear(
        reinterpret_cast<RemoveAllQuestInfoElements_GlobalOverlay*>(DAT_013b8790)->m_pDrawQuestInfo);
}