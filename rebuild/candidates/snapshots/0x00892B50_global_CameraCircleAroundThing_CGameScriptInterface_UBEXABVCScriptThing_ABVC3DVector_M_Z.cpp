#include <cstddef>
#include <cstdint>

struct C3DVector
{
    float x;
    float y;
    float z;
};

class CScriptThing;
class CGameCameraManager;
class CGameScriptInterface;

namespace NCameraModes
{
class CScriptModuleBase
{
public:
    CScriptModuleBase();
};
}

class CGameCameraManager
{
public:
    void __thiscall SetScriptedCameraMode(
        NCameraModes::CScriptModuleBase* scriptModule,
        void* callbackState);
};

namespace
{
struct CGameScriptInterface_CameraCircleAroundThing_Overlay
{
    std::byte pad_0000[0x04];
    void* field_0004;
};

static_assert(offsetof(CGameScriptInterface_CameraCircleAroundThing_Overlay, field_0004) == 0x04);

struct CGameScriptInterface_CameraCircleAroundThing_Field0004_Overlay
{
    std::byte pad_0000[0x34];
    void* field_0034;
};

static_assert(
    offsetof(CGameScriptInterface_CameraCircleAroundThing_Field0004_Overlay, field_0034) == 0x34);

struct CameraCircleAroundThing_Module_Overlay
{
    void** vftable;
    std::byte pad_0004[0x48];
    std::uint32_t field_004C;
    float field_0050;
    float field_0054;
    float field_0058;
    float field_005C;
};

static_assert(offsetof(CameraCircleAroundThing_Module_Overlay, field_004C) == 0x4C);
static_assert(offsetof(CameraCircleAroundThing_Module_Overlay, field_0050) == 0x50);
static_assert(offsetof(CameraCircleAroundThing_Module_Overlay, field_0054) == 0x54);
static_assert(offsetof(CameraCircleAroundThing_Module_Overlay, field_0058) == 0x58);
static_assert(offsetof(CameraCircleAroundThing_Module_Overlay, field_005C) == 0x5C);
static_assert(sizeof(CameraCircleAroundThing_Module_Overlay) == 0x60);

struct CameraCircleAroundThing_SetScriptedCameraMode_CallTuple
{
    NCameraModes::CScriptModuleBase* scriptModule;
    void* callbackState;
};

static_assert(
    offsetof(CameraCircleAroundThing_SetScriptedCameraMode_CallTuple, scriptModule) == 0x00);
static_assert(
    offsetof(CameraCircleAroundThing_SetScriptedCameraMode_CallTuple, callbackState) == 0x04);
static_assert(sizeof(CameraCircleAroundThing_SetScriptedCameraMode_CallTuple) == 0x08);

struct CameraCircleAroundThing_HeapCallbackBlock
{
    std::int32_t field_0000;
    void* field_0004;
    NCameraModes::CScriptModuleBase* field_0008;
};

static_assert(offsetof(CameraCircleAroundThing_HeapCallbackBlock, field_0000) == 0x00);
static_assert(offsetof(CameraCircleAroundThing_HeapCallbackBlock, field_0004) == 0x04);
static_assert(offsetof(CameraCircleAroundThing_HeapCallbackBlock, field_0008) == 0x08);
static_assert(sizeof(CameraCircleAroundThing_HeapCallbackBlock) == 0x0C);

extern void* PTR__vector_deleting_destructor__01278188;
extern void* LAB_005ecf50;

void* __cdecl operator_new(std::size_t size);
}

class CGameScriptInterface
{
public:
    void __thiscall CameraCircleAroundThing(
        const CScriptThing& thing,
        const C3DVector& position,
        float value) const
    {
        using ResolveCameraManagerFn = void(__thiscall*)(void*, void**);
        using GetThingValueFn = std::uint32_t(__thiscall*)(const CScriptThing*);

        void* resolvedManagerStorage = const_cast<CGameScriptInterface*>(this);

        const auto* const overlay =
            reinterpret_cast<const CGameScriptInterface_CameraCircleAroundThing_Overlay*>(this);
        const auto* const field0004 =
            reinterpret_cast<const CGameScriptInterface_CameraCircleAroundThing_Field0004_Overlay*>(
                overlay->field_0004);
        void* const resolverObject = field0004->field_0034;

        const auto resolveCameraManager = reinterpret_cast<ResolveCameraManagerFn>(
            (*reinterpret_cast<void***>(resolverObject))[0x24 / sizeof(void*)]);
        resolveCameraManager(resolverObject, &resolvedManagerStorage);

        auto* const cameraManager =
            reinterpret_cast<CGameCameraManager*>(resolvedManagerStorage);

        auto* module = static_cast<CameraCircleAroundThing_Module_Overlay*>(operator_new(0x60));
        if (module != nullptr)
        {
            const auto getThingValue = reinterpret_cast<GetThingValueFn>(
                (*reinterpret_cast<void***>(const_cast<CScriptThing*>(&thing)))[0x2C / sizeof(void*)]);
            const std::uint32_t thingValue = getThingValue(&thing);

            NCameraModes::CScriptModuleBase::CScriptModuleBase(
                reinterpret_cast<NCameraModes::CScriptModuleBase*>(module));
            module->vftable = reinterpret_cast<void**>(&PTR__vector_deleting_destructor__01278188);
            module->field_004C = thingValue;
            module->field_0050 = position.x;
            module->field_0054 = position.y;
            module->field_0058 = position.z;
            module->field_005C = value;
        }

        CameraCircleAroundThing_SetScriptedCameraMode_CallTuple callTuple;
        if (module != nullptr)
        {
            auto* callbackBlock =
                static_cast<CameraCircleAroundThing_HeapCallbackBlock*>(operator_new(0x0C));
            if (callbackBlock != nullptr)
            {
                callbackBlock->field_0000 = 1;
                callbackBlock->field_0004 = &LAB_005ecf50;
                callbackBlock->field_0008 =
                    reinterpret_cast<NCameraModes::CScriptModuleBase*>(module);

                callTuple.scriptModule =
                    reinterpret_cast<NCameraModes::CScriptModuleBase*>(module);
                callTuple.callbackState = callbackBlock;
                cameraManager->SetScriptedCameraMode(
                    callTuple.scriptModule,
                    callTuple.callbackState);
                return;
            }

            callTuple.scriptModule =
                reinterpret_cast<NCameraModes::CScriptModuleBase*>(module);
            callTuple.callbackState = nullptr;
            cameraManager->SetScriptedCameraMode(
                callTuple.scriptModule,
                callTuple.callbackState);
            return;
        }

        callTuple.scriptModule = nullptr;
        callTuple.callbackState = nullptr;
        cameraManager->SetScriptedCameraMode(
            callTuple.scriptModule,
            callTuple.callbackState);
    }
};