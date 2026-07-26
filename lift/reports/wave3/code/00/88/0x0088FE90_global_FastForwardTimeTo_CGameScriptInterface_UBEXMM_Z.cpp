#include <cstddef>

class CGameTimeManager
{
public:
    void __thiscall FastForwardTimeTo(float timeOfDay, float speed);
};

class CGameScriptInterface
{
public:
    void __thiscall FastForwardTimeTo(float timeOfDay, float speed) const;
};

namespace
{
struct CGameScriptInterface_FastForwardTimeTo_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_FastForwardTimeTo_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_FastForwardTimeTo_Field04Overlay
{
    std::byte m_Pad00[0x1C];
    CGameTimeManager* m_Field1C; // 0x1C
};
static_assert(
    offsetof(
        CGameScriptInterface_FastForwardTimeTo_Field04Overlay,
        m_Field1C) == 0x1C);

float& g_FastForwardTimeTo_WrapValue = *reinterpret_cast<float*>(0x0123822C);
float& g_FastForwardTimeTo_TimeScale = *reinterpret_cast<float*>(0x0122DC88);
}

void __thiscall CGameScriptInterface::FastForwardTimeTo(
    float timeOfDay,
    float speed) const
{
    for (; g_FastForwardTimeTo_WrapValue <= timeOfDay;
         timeOfDay = timeOfDay - g_FastForwardTimeTo_WrapValue)
    {
    }

    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_FastForwardTimeTo_SelfOverlay*>(this);
    const auto* const field04 =
        reinterpret_cast<const CGameScriptInterface_FastForwardTimeTo_Field04Overlay*>(
            self->m_Field04);

    field04->m_Field1C->FastForwardTimeTo(
        timeOfDay * g_FastForwardTimeTo_TimeScale,
        speed);
}