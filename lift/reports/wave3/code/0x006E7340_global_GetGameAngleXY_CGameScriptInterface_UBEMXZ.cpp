#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    float __thiscall GetGameAngleXY() const;
};

namespace
{
struct CGameScriptInterface_GetGameAngleXY_ThisOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_GetGameAngleXY_ThisOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_GetGameAngleXY_Field04Overlay
{
    std::byte m_Pad00[0x34];
    void* m_Field34; // 0x34
};
static_assert(offsetof(CGameScriptInterface_GetGameAngleXY_Field04Overlay, m_Field34) == 0x34);

using CGameScriptInterface_GetGameAngleXY_ProviderCall =
    std::uint8_t(__thiscall*)(void*, void**);
using CGameScriptInterface_GetGameAngleXY_AngleCall =
    float(__thiscall*)(void*);

auto* const DAT_0122DEDC =
    reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetGameAngleXY() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetGameAngleXY_ThisOverlay*>(this);
    auto* const field04 =
        reinterpret_cast<CGameScriptInterface_GetGameAngleXY_Field04Overlay*>(self->m_Field04);
    void* const provider = field04->m_Field34;

    void* targetObject;
    const auto providerCall =
        reinterpret_cast<CGameScriptInterface_GetGameAngleXY_ProviderCall>(
            (*reinterpret_cast<void***>(provider))[0x28 / sizeof(void*)]);

    if (providerCall(provider, &targetObject) != 0)
    {
        const auto angleCall =
            reinterpret_cast<CGameScriptInterface_GetGameAngleXY_AngleCall>(
                (*reinterpret_cast<void***>(targetObject))[0x64 / sizeof(void*)]);
        return angleCall(targetObject);
    }

    return *DAT_0122DEDC;
}