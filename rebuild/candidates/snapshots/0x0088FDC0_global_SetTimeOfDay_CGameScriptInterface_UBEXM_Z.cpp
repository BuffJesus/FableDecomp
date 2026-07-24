#include <cstddef>

class CThingDrawInformation
{
public:
    void __thiscall SetBarPercentage(float percentage);
};

class CGameScriptInterface
{
public:
    void __thiscall SetTimeOfDay(float timeOfDay) const;
};

namespace
{
struct CGameScriptInterface_SetTimeOfDay_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetTimeOfDay_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_SetTimeOfDay_Field04Overlay
{
    std::byte m_Pad00[0x1C];
    CThingDrawInformation* m_Field1C; // 0x1C
};
static_assert(
    offsetof(
        CGameScriptInterface_SetTimeOfDay_Field04Overlay,
        m_Field1C) == 0x1C);

float& g_SetTimeOfDay_WrapValue = *reinterpret_cast<float*>(0x0123822C);
float& g_SetTimeOfDay_BarScale = *reinterpret_cast<float*>(0x0122DC88);
}

void __thiscall CGameScriptInterface::SetTimeOfDay(float timeOfDay) const
{
    for (; g_SetTimeOfDay_WrapValue <= timeOfDay; timeOfDay = timeOfDay - g_SetTimeOfDay_WrapValue)
    {
    }

    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_SetTimeOfDay_SelfOverlay*>(this);
    const auto* const field04 =
        reinterpret_cast<const CGameScriptInterface_SetTimeOfDay_Field04Overlay*>(self->m_Field04);

    field04->m_Field1C->SetBarPercentage(timeOfDay * g_SetTimeOfDay_BarScale);
}