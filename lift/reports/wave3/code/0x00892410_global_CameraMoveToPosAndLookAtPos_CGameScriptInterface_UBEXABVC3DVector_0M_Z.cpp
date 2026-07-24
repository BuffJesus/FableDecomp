#include <cstddef>
#include <cstdint>
#include <new>

struct C3DVector
{
    float x;
    float y;
    float z;
};

class C3DPositionInterpolator
{
public:
    std::byte pad_0000[0x6C];

    C3DPositionInterpolator(const C3DVector& position, const C3DVector& lookAtPosition);
};

static_assert(sizeof(C3DPositionInterpolator) == 0x6C);

class CGameScriptInterface
{
public:
    void __thiscall CameraMoveToPosAndLookAtPos(
        const C3DVector& position,
        const C3DVector& lookAtPosition,
        float duration) const;
};

void* __cdecl operator_new(std::size_t size);
extern void* LAB_005ecf50;

namespace
{
struct CGameScriptInterfaceOverlay
{
    std::byte pad_0000[0x04];
    void* field_0004;
};
static_assert(offsetof(CGameScriptInterfaceOverlay, field_0004) == 0x04);

struct Field0004Overlay
{
    std::byte pad_0000[0x34];
    void* field_0034;
};
static_assert(offsetof(Field0004Overlay, field_0034) == 0x34);

struct VtableObjectOverlay
{
    void** vftable;
};
static_assert(offsetof(VtableObjectOverlay, vftable) == 0x00);

struct ScriptedCameraModeCallbackState
{
    std::int32_t field_0000;
    void* field_0004;
    void* field_0008;
};
static_assert(offsetof(ScriptedCameraModeCallbackState, field_0000) == 0x00);
static_assert(offsetof(ScriptedCameraModeCallbackState, field_0004) == 0x04);
static_assert(offsetof(ScriptedCameraModeCallbackState, field_0008) == 0x08);
static_assert(sizeof(ScriptedCameraModeCallbackState) == 0x0C);

struct ScriptedCameraModeReceiver
{
    void __thiscall SetScriptedCameraMode(void* scriptedCameraMode, void* callbackState);
};
}

void __thiscall CGameScriptInterface::CameraMoveToPosAndLookAtPos(
    const C3DVector& position,
    const C3DVector& lookAtPosition,
    float) const
{
    void* local_4 = const_cast<CGameScriptInterface*>(this);

    const auto* const selfOverlay =
        reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    const auto* const field0004Overlay =
        reinterpret_cast<const Field0004Overlay*>(selfOverlay->field_0004);
    auto* const resolverObject =
        reinterpret_cast<VtableObjectOverlay*>(field0004Overlay->field_0034);

    using ResolverSlot24 = void(__thiscall*)(void*, void**);
    const auto resolverSlot24 =
        reinterpret_cast<ResolverSlot24>(resolverObject->vftable[0x24 / sizeof(void*)]);
    resolverSlot24(resolverObject, &local_4);

    auto* const interpolatorStorage =
        static_cast<C3DPositionInterpolator*>(operator_new(0x6C));

    C3DPositionInterpolator* iVar1;
    if (interpolatorStorage == nullptr)
    {
        iVar1 = nullptr;
    }
    else
    {
        iVar1 = ::new (static_cast<void*>(interpolatorStorage))
            C3DPositionInterpolator(position, lookAtPosition);
    }

    if (iVar1 != nullptr)
    {
        auto* const puVar2 =
            static_cast<ScriptedCameraModeCallbackState*>(operator_new(0x0C));
        if (puVar2 != nullptr)
        {
            puVar2->field_0000 = 1;
            puVar2->field_0004 = &LAB_005ecf50;
            puVar2->field_0008 = iVar1;
            reinterpret_cast<ScriptedCameraModeReceiver*>(local_4)
                ->SetScriptedCameraMode(iVar1, puVar2);
            return;
        }

        reinterpret_cast<ScriptedCameraModeReceiver*>(local_4)
            ->SetScriptedCameraMode(iVar1, nullptr);
        return;
    }

    reinterpret_cast<ScriptedCameraModeReceiver*>(local_4)
        ->SetScriptedCameraMode(nullptr, nullptr);
}