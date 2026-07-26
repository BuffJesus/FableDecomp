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

class CGameScriptInterface
{
public:
    void __thiscall CameraCircleAroundPos(
        const C3DVector& vector0,
        const C3DVector& vector1,
        float value) const;
};

void* __cdecl operator_new(std::size_t size);
extern void* PTR__vector_deleting_destructor__01278160;

namespace
{
struct CGameScriptInterface_CameraCircleAroundPos_ThisOverlay
{
    std::byte pad_0000[0x04];
    void* field_0004;
};

static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_ThisOverlay, field_0004) == 0x04);

struct CGameScriptInterface_CameraCircleAroundPos_Field04Overlay
{
    std::byte pad_0000[0x34];
    void* field_0034;
};

static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_Field04Overlay, field_0034) == 0x34);

struct CGameScriptInterface_CameraCircleAroundPos_ScriptModuleOverlay
{
    std::byte pad_0000[0x4C];
    C3DVector field_004C;
    C3DVector field_0058;
    float field_0064;
};

static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_ScriptModuleOverlay, field_004C) == 0x4C);
static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_ScriptModuleOverlay, field_0058) == 0x58);
static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_ScriptModuleOverlay, field_0064) == 0x64);
static_assert(sizeof(CGameScriptInterface_CameraCircleAroundPos_ScriptModuleOverlay) == 0x68);

struct CGameScriptInterface_CameraCircleAroundPos_CallbackOverlay
{
    std::int32_t field_0000;
    void* field_0004;
    NCameraModes::CScriptModuleBase* field_0008;
};

static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_CallbackOverlay, field_0000) == 0x00);
static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_CallbackOverlay, field_0004) == 0x04);
static_assert(offsetof(CGameScriptInterface_CameraCircleAroundPos_CallbackOverlay, field_0008) == 0x08);
static_assert(sizeof(CGameScriptInterface_CameraCircleAroundPos_CallbackOverlay) == 0x0C);

constexpr std::uintptr_t CGameScriptInterface_CameraCircleAroundPos_CallbackThunk = 0x005ECF50;

__declspec(naked) CGameCameraManager* __cdecl
CGameScriptInterface_CameraCircleAroundPos_CallProviderSlot24AndCaptureEsi(
    void* cameraProvider,
    void* local4)
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
}

void __thiscall CGameScriptInterface::CameraCircleAroundPos(
    const C3DVector& vector0,
    const C3DVector& vector1,
    float value) const
{
    std::uintptr_t local_4 = reinterpret_cast<std::uintptr_t>(this);

    auto* const thisOverlay =
        reinterpret_cast<const CGameScriptInterface_CameraCircleAroundPos_ThisOverlay*>(this);
    auto* const field04Overlay =
        reinterpret_cast<CGameScriptInterface_CameraCircleAroundPos_Field04Overlay*>(thisOverlay->field_0004);
    void* const cameraProvider = field04Overlay->field_0034;

    CGameCameraManager* const cameraManager =
        CGameScriptInterface_CameraCircleAroundPos_CallProviderSlot24AndCaptureEsi(
            cameraProvider,
            &local_4);

    auto* scriptModule =
        static_cast<CGameScriptInterface_CameraCircleAroundPos_ScriptModuleOverlay*>(operator_new(0x68));
    if (scriptModule != nullptr)
    {
        ::new (static_cast<void*>(scriptModule)) NCameraModes::CScriptModuleBase();
        *reinterpret_cast<void***>(scriptModule) =
            reinterpret_cast<void**>(&PTR__vector_deleting_destructor__01278160);
        scriptModule->field_004C = vector0;
        scriptModule->field_0058 = vector1;
        scriptModule->field_0064 = value;
    }

    if (scriptModule != nullptr)
    {
        auto* callbackState =
            static_cast<CGameScriptInterface_CameraCircleAroundPos_CallbackOverlay*>(operator_new(0x0C));
        if (callbackState != nullptr)
        {
            callbackState->field_0000 = 1;
            callbackState->field_0004 =
                reinterpret_cast<void*>(CGameScriptInterface_CameraCircleAroundPos_CallbackThunk);
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