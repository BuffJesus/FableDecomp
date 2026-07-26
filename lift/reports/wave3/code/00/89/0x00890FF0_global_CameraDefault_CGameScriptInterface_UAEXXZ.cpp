#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CGameCameraManager;

class CGameCameraManager
{
public:
    void __thiscall ResetCamera();
    void __thiscall ReleaseScriptedControl();
};

class CGameScriptInterface
{
public:
    void __thiscall CameraDefault();
};

namespace
{
struct CGameScriptInterface_CameraDefault_GlobalOverlay
{
    std::byte m_Pad00[0x1C];
    void* m_Field1C; // 0x1C
};
static_assert(offsetof(CGameScriptInterface_CameraDefault_GlobalOverlay, m_Field1C) == 0x1C);

struct CGameScriptInterface_CameraDefault_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_CameraDefault_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_CameraDefault_Field04Overlay
{
    std::byte m_Pad00[0x34];
    void* m_Field34; // 0x34
};
static_assert(offsetof(CGameScriptInterface_CameraDefault_Field04Overlay, m_Field34) == 0x34);

struct CGameScriptInterface_CameraDefault_SelfOverlay
{
    std::byte m_Pad00[0x04];
    CGameScriptInterface_CameraDefault_Field04Overlay* m_Field04; // 0x04
    std::byte m_Pad08[0x40];
    std::int32_t m_Field48; // 0x48
};
static_assert(offsetof(CGameScriptInterface_CameraDefault_SelfOverlay, m_Field04) == 0x04);
static_assert(offsetof(CGameScriptInterface_CameraDefault_SelfOverlay, m_Field48) == 0x48);

using CGameScriptInterface_CameraDefault_Helper00449970Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_CameraDefault_Helper00487DC0Fn =
    CGameScriptInterface_CameraDefault_TargetOverlay*(__thiscall*)(void*);
using CGameScriptInterface_CameraDefault_GetCameraManagerFn =
    void(__thiscall*)(void*, CGameCameraManager**);
using CGameScriptInterface_CameraDefault_RawThisSlot1CFn =
    void(__thiscall*)(CGameScriptInterface*);

auto* const CGameScriptInterface_CameraDefault_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_CameraDefault_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_CameraDefault_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_CameraDefault_Helper00487DC0Fn>(0x00487DC0);
auto* const g_CGameScriptInterface_CameraDefault_Global =
    reinterpret_cast<CGameScriptInterface_CameraDefault_GlobalOverlay**>(0x013B86A0);
}

void __thiscall CGameScriptInterface::CameraDefault()
{
    auto* const self =
        reinterpret_cast<CGameScriptInterface_CameraDefault_SelfOverlay*>(this);
    auto* const helperResult =
        CGameScriptInterface_CameraDefault_Helper00449970(
            (*g_CGameScriptInterface_CameraDefault_Global)->m_Field1C);
    auto* const target =
        CGameScriptInterface_CameraDefault_Helper00487DC0(helperResult);

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        CGameCameraManager* pCameraManager;
        auto* const cameraManagerProvider = self->m_Field04->m_Field34;
        auto* const cameraManagerProviderVftable =
            *reinterpret_cast<void***>(cameraManagerProvider);

        reinterpret_cast<CGameScriptInterface_CameraDefault_GetCameraManagerFn>(
            cameraManagerProviderVftable[9])(cameraManagerProvider, &pCameraManager);

        if (self->m_Field48 >= 0)
        {
            pCameraManager->ResetCamera();

            do
            {
                auto* const selfVftable = *reinterpret_cast<void***>(this);

                reinterpret_cast<CGameScriptInterface_CameraDefault_RawThisSlot1CFn>(
                    selfVftable[7])(this);

                const std::int32_t field48 = self->m_Field48;
                self->m_Field48 = field48 - 1;
            } while ((field48 - 1) != -1);

            self->m_Field48 = -1;
            return;
        }

        pCameraManager->ReleaseScriptedControl();
        self->m_Field48 = -1;
    }
}