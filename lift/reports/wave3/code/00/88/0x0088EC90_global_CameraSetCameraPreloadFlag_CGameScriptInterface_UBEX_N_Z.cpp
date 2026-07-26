#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall CameraSetCameraPreloadFlag(bool preloadFlag) const;
};

namespace
{
struct CGameScriptInterface_CameraSetCameraPreloadFlag_ThisOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_CameraSetCameraPreloadFlag_ThisOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_CameraSetCameraPreloadFlag_Field04Overlay
{
    std::byte m_Pad00[0x34];
    void* m_Field34; // 0x34
};
static_assert(offsetof(CGameScriptInterface_CameraSetCameraPreloadFlag_Field04Overlay, m_Field34) == 0x34);

struct CGameScriptInterface_CameraSetCameraPreloadFlag_TargetOverlay
{
    std::byte m_Pad00[0x145];
    std::uint8_t m_PreloadFlag; // 0x145
};
static_assert(offsetof(CGameScriptInterface_CameraSetCameraPreloadFlag_TargetOverlay, m_PreloadFlag) == 0x145);

using CameraSetCameraPreloadFlag_IndirectCall = void(__thiscall*)(void*, void**);
}

void __thiscall CGameScriptInterface::CameraSetCameraPreloadFlag(bool preloadFlag) const
{
    auto* const self =
        reinterpret_cast<const CGameScriptInterface_CameraSetCameraPreloadFlag_ThisOverlay*>(this);
    auto* const field04 =
        reinterpret_cast<CGameScriptInterface_CameraSetCameraPreloadFlag_Field04Overlay*>(self->m_Field04);
    void* const targetObjectSource = field04->m_Field34;

    void* targetObject;
    auto* const vtable = *reinterpret_cast<void***>(targetObjectSource);
    const auto indirectCall =
        reinterpret_cast<CameraSetCameraPreloadFlag_IndirectCall>(vtable[0x24 / sizeof(void*)]);
    indirectCall(targetObjectSource, &targetObject);

    reinterpret_cast<CGameScriptInterface_CameraSetCameraPreloadFlag_TargetOverlay*>(targetObject)->m_PreloadFlag =
        static_cast<std::uint8_t>(preloadFlag);
}