#include <cstddef>
#include <cstdint>

class CCharString;
class CRGBColour;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long __thiscall AddBar(
        float value,
        float maxValue,
        const CRGBColour& filledColour,
        const CRGBColour& emptyColour,
        const CCharString& labelText,
        const CCharString& valueText,
        float fadeTime);
};
}

class CGameScriptInterface
{
public:
    long __thiscall AddQuestInfoBar(
        float value,
        float maxValue,
        const CRGBColour& filledColour,
        const CRGBColour& emptyColour,
        const CCharString& labelText,
        const CCharString& valueText,
        float fadeTime) const;
};

extern "C" std::uint32_t DAT_013b8790;

namespace
{
struct AddQuestInfoBar_GlobalOverlay
{
    std::byte m_Pad00[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_pDrawQuestInfo; // 0x1C8
};

static_assert(offsetof(AddQuestInfoBar_GlobalOverlay, m_pDrawQuestInfo) == 0x1C8);
}

long __thiscall CGameScriptInterface::AddQuestInfoBar(
    float value,
    float maxValue,
    const CRGBColour& filledColour,
    const CRGBColour& emptyColour,
    const CCharString& labelText,
    const CCharString& valueText,
    float fadeTime) const
{
    return NPlayerGui::CDrawQuestInfo::AddBar(
        reinterpret_cast<AddQuestInfoBar_GlobalOverlay*>(DAT_013b8790)->m_pDrawQuestInfo,
        value,
        maxValue,
        filledColour,
        emptyColour,
        labelText,
        valueText,
        fadeTime);
}