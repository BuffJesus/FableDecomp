#include <cstddef>
#include <cstdint>
#include <new>

struct C3DVector
{
    float x;
    float y;
    float z;
};

class CGameCameraManager;
class CGameScriptInterface;

namespace NCameraModes
{
class CScriptModuleBase;

class CScriptModuleFullTransition
{
public:
    std::byte pad_0000[0x80];

    CScriptModuleFullTransition(
        const C3DVector& arg0,
        const C3DVector& arg1,
        const C3DVector& arg2,
        const C3DVector& arg3,
        std::int32_t duration);
};

static_assert(sizeof(CScriptModuleFullTransition) == 0x80);
}

class CGameCameraManager
{
public:
    void __thiscall SetScriptedCameraMode(
        NCameraModes::CScriptModuleBase* scriptModule,
        void* callbackState);
};

class CGameScriptInterface
{
public:
    void __thiscall CameraMoveBetweenLookFromAndLookTo(
        const C3DVector& arg0,
        const C3DVector& arg1,
        const C3DVector& arg2,
        const C3DVector& arg3,
        float duration) const;
};

extern "C" std::int32_t __cdecl __ftol2();
void* __cdecl operator_new(std::size_t size);
extern void* LAB_005ecf50;

namespace
{
inline const std::int32_t& _DAT_01375550 = *reinterpret_cast<const std::int32_t*>(0x01375550);

struct CGameScriptInterface_SelfOverlay
{
    std::byte pad_0000[0x04];
    void* field_0004;
};
static_assert(offsetof(CGameScriptInterface_SelfOverlay, field_0004) == 0x04);

struct CGameScriptInterface_Field0004Overlay
{
    std::byte pad_0000[0x34];
    void* field_0034;
};
static_assert(offsetof(CGameScriptInterface_Field0004Overlay, field_0034) == 0x34);

struct ScriptedCameraModeCallbackState
{
    std::int32_t field_0000;
    void* field_0004;
    NCameraModes::CScriptModuleBase* field_0008;
};
static_assert(offsetof(ScriptedCameraModeCallbackState, field_0000) == 0x00);
static_assert(offsetof(ScriptedCameraModeCallbackState, field_0004) == 0x04);
static_assert(offsetof(ScriptedCameraModeCallbackState, field_0008) == 0x08);
static_assert(sizeof(ScriptedCameraModeCallbackState) == 0x0C);

__declspec(naked) CGameCameraManager* __cdecl ResolveCameraManager(
    void* cameraProvider,
    void* localThisStorage)
{
    __asm
    {
        mov ecx, [esp + 4]
        mov eax, [ecx]
        push dword ptr [esp + 8]
        call dword ptr [eax + 24h]
        mov eax, esi
        ret 8
    }
}

__declspec(naked) std::int32_t __cdecl ConvertDuration(float duration)
{
    __asm
    {
        fild dword ptr [_DAT_01375550]
        fmul dword ptr [esp + 4]
        call __ftol2
        ret 4
    }
}
}

void __thiscall CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo(
    const C3DVector& arg0,
    const C3DVector& arg1,
    const C3DVector& arg2,
    const C3DVector& arg3,
    float duration) const
{
    std::int32_t local_4 = reinterpret_cast<std::int32_t>(this);

    const auto* const self = reinterpret_cast<const CGameScriptInterface_SelfOverlay*>(this);
    const auto* const field0004 =
        reinterpret_cast<const CGameScriptInterface_Field0004Overlay*>(self->field_0004);

    CGameCameraManager* const cameraManager =
        ResolveCameraManager(field0004->field_0034, &local_4);

    auto* scriptModule =
        static_cast<NCameraModes::CScriptModuleFullTransition*>(operator_new(0x80));
    if (scriptModule != nullptr)
    {
        const std::int32_t convertedDuration = ConvertDuration(duration);

        ::new (static_cast<void*>(scriptModule)) NCameraModes::CScriptModuleFullTransition(
            arg1,
            arg3,
            arg2,
            arg0,
            convertedDuration);
    }

    if (scriptModule != nullptr)
    {
        auto* callbackState =
            static_cast<ScriptedCameraModeCallbackState*>(operator_new(0x0C));
        if (callbackState != nullptr)
        {
            callbackState->field_0000 = 1;
            callbackState->field_0004 = &LAB_005ecf50;
            callbackState->field_0008 =
                reinterpret_cast<NCameraModes::CScriptModuleBase*>(scriptModule);

            cameraManager->SetScriptedCameraMode(
                reinterpret_cast<NCameraModes::CScriptModuleBase*>(scriptModule),
                callbackState);
            return;
        }

        cameraManager->SetScriptedCameraMode(
            reinterpret_cast<NCameraModes::CScriptModuleBase*>(scriptModule),
            nullptr);
        return;
    }

    cameraManager->SetScriptedCameraMode(nullptr, nullptr);
}