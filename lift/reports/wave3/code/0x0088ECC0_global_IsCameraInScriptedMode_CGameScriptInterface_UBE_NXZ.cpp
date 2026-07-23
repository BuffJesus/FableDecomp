#include <cstddef>
#include <cstdint>

class CGameCameraManager
{
public:
    bool __thiscall IsUsingScriptedCameraMode();
};

struct CGameScriptInterface_0x04_Overlay
{
    std::byte pad_0x00[0x04];
    void* field_0x04;
};

static_assert(offsetof(CGameScriptInterface_0x04_Overlay, field_0x04) == 0x04);

struct UnknownInterfaceOwner_0x34_Overlay
{
    std::byte pad_0x00[0x34];
    void* field_0x34;
};

static_assert(offsetof(UnknownInterfaceOwner_0x34_Overlay, field_0x34) == 0x34);

class CGameScriptInterface
{
public:
    bool __thiscall IsCameraInScriptedMode() const;
};

bool __thiscall CGameScriptInterface::IsCameraInScriptedMode() const
{
    using GetCameraManagerFn = void(__thiscall*)(void* pThis, CGameCameraManager** ppCameraManager);

    CGameCameraManager* camera_manager =
        reinterpret_cast<CGameCameraManager*>(const_cast<CGameScriptInterface*>(this));

    auto* const overlay = reinterpret_cast<CGameScriptInterface_0x04_Overlay*>(
        const_cast<CGameScriptInterface*>(this));
    auto* const owner = reinterpret_cast<UnknownInterfaceOwner_0x34_Overlay*>(overlay->field_0x04);
    void* const interface_object = owner->field_0x34;

    const auto get_camera_manager =
        reinterpret_cast<GetCameraManagerFn>((*reinterpret_cast<void***>(interface_object))[9]);
    get_camera_manager(interface_object, &camera_manager);

    return camera_manager->IsUsingScriptedCameraMode();
}