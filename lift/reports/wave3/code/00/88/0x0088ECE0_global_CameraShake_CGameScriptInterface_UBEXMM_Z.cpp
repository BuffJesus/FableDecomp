#include <cstddef>
#include <cstdint>

struct C3DVector
{
    float x;
    float y;
    float z;
};

class CGameCameraManager;
class CGameScriptInterface
{
public:
    void __thiscall CameraShake(float magnitude, float duration) const;
};

namespace
{
struct CGameScriptInterface_CameraShake_ThisOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_CameraShake_ThisOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_CameraShake_Field04Overlay
{
    std::byte m_Pad00[0x34];
    void* m_Field34; // 0x34
};
static_assert(offsetof(CGameScriptInterface_CameraShake_Field04Overlay, m_Field34) == 0x34);

struct CGameScriptInterface_CameraShake_CameraManagerOverlay
{
    std::byte m_Pad00[0x04];
    C3DVector m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_CameraShake_CameraManagerOverlay, m_Field04) == 0x04);

using CGameScriptInterface_CameraShake_GetCameraManagerFn =
    std::uint8_t(__thiscall*)(void*, CGameCameraManager**);
using CGameScriptInterface_CameraShake_SetEarthquakeAtFn =
    void(__thiscall*)(CGameCameraManager*, C3DVector*, float, float, bool);

auto* const CGameScriptInterface_CameraShake_SetEarthquakeAt =
    reinterpret_cast<CGameScriptInterface_CameraShake_SetEarthquakeAtFn>(0x00699560);
}

void __thiscall CGameScriptInterface::CameraShake(float magnitude, float duration) const
{
    auto* const self =
        reinterpret_cast<const CGameScriptInterface_CameraShake_ThisOverlay*>(this);
    auto* const field04 =
        reinterpret_cast<CGameScriptInterface_CameraShake_Field04Overlay*>(self->m_Field04);
    void* const cameraManagerProvider = field04->m_Field34;

    CGameCameraManager* cameraManager;
    const auto getCameraManager =
        reinterpret_cast<CGameScriptInterface_CameraShake_GetCameraManagerFn>(
            (*reinterpret_cast<void***>(cameraManagerProvider))[0x24 / sizeof(void*)]);

    if (getCameraManager(cameraManagerProvider, &cameraManager) != 0)
    {
        auto* const cameraManagerOverlay =
            reinterpret_cast<CGameScriptInterface_CameraShake_CameraManagerOverlay*>(cameraManager);

        CGameScriptInterface_CameraShake_SetEarthquakeAt(
            cameraManager,
            &cameraManagerOverlay->m_Field04,
            magnitude,
            duration,
            false);
    }
}