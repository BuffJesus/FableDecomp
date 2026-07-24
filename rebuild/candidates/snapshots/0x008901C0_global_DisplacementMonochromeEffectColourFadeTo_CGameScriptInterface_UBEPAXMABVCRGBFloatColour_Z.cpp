#include <cstddef>

class CCharString;
class CRGBFloatColour;

extern "C" void* __thiscall std__vector_CCharString__allocator_CCharString_____Ucopy(
    void* pVector,
    float duration,
    int count,
    const CRGBFloatColour& colour);

class CGameScriptInterface
{
public:
    void* __thiscall DisplacementMonochromeEffectColourFadeTo(
        float duration,
        const CRGBFloatColour& colour) const;
};

namespace
{
struct CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Field18_Overlay
{
    std::byte m_Pad00[0x1964];
    void* m_Field1964;
};
static_assert(offsetof(CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Field18_Overlay, m_Field1964) == 0x1964);

struct CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Field04_Overlay
{
    std::byte m_Pad00[0x18];
    CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Field18_Overlay* m_Field18;
};
static_assert(offsetof(CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Field04_Overlay, m_Field18) == 0x18);

struct CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Overlay
{
    std::byte m_Pad00[0x4];
    CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Field04_Overlay* m_Field04;
};
static_assert(offsetof(CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Overlay, m_Field04) == 0x4);
}

void* __thiscall CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo(
    float duration,
    const CRGBFloatColour& colour) const
{
    const auto* const pThis =
        reinterpret_cast<const CGameScriptInterface_DisplacementMonochromeEffectColourFadeTo_Overlay*>(this);

    return std__vector_CCharString__allocator_CCharString_____Ucopy(
        pThis->m_Field04->m_Field18->m_Field1964,
        duration,
        1,
        colour);
}