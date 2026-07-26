#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CRightHandedSet;
class CScriptThing;
class CThing;

struct C3DVector
{
    float x;
    float y;
    float z;
};
static_assert(sizeof(C3DVector) == 0x0C);

namespace
{
using ResolveThingFn = CThing* (__thiscall*)(const CScriptThing*);

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    ResolveThingFn m_ResolveThing; // 0x2C
};
static_assert(offsetof(CScriptThingVTableOverlay, m_ResolveThing) == 0x2C);

struct CScriptThingOverlay
{
    CScriptThingVTableOverlay* m_pVTable; // 0x00
};

using GetRightHandedSetFn =
    const CRightHandedSet* (__thiscall*)(const void*);

struct CameraPointComponentVTableOverlay
{
    std::byte m_Pad00[0x120];
    GetRightHandedSetFn m_GetRightHandedSet; // 0x120
};
static_assert(
    offsetof(CameraPointComponentVTableOverlay, m_GetRightHandedSet) == 0x120);

struct CameraPointComponentOverlay
{
    CameraPointComponentVTableOverlay* m_pVTable; // 0x00
    std::byte m_Pad04[0x08];
    C3DVector m_Position; // 0x0C
};
static_assert(offsetof(CameraPointComponentOverlay, m_Position) == 0x0C);

struct CThingOverlay
{
    std::byte m_Pad00[0x60];
    CameraPointComponentOverlay* m_pCameraPoint; // 0x60
};
static_assert(offsetof(CThingOverlay, m_pCameraPoint) == 0x60);

using CameraUsePositionFn = void (__thiscall*)(
    const CGameScriptInterface*,
    const CScriptThing&,
    const C3DVector*,
    const CRightHandedSet*,
    float,
    long,
    long);

struct CGameScriptInterfaceVTableOverlay
{
    std::byte m_Pad00[0x66C];
    CameraUsePositionFn m_CameraUsePosition; // 0x66C
};
static_assert(
    offsetof(CGameScriptInterfaceVTableOverlay, m_CameraUsePosition) == 0x66C);

struct CGameScriptInterfaceOverlay
{
    CGameScriptInterfaceVTableOverlay* m_pVTable; // 0x00
};
}

class CGameScriptInterface
{
public:
    void __thiscall CameraUseCameraPoint(
        const CScriptThing& cameraTarget,
        const CScriptThing& cameraPoint,
        float transitionTime,
        long parameter1,
        long parameter2) const;
};

void __thiscall CGameScriptInterface::CameraUseCameraPoint(
    const CScriptThing& cameraTarget,
    const CScriptThing& cameraPoint,
    float transitionTime,
    long parameter1,
    long parameter2) const
{
    const auto* const scriptThing =
        reinterpret_cast<const CScriptThingOverlay*>(&cameraPoint);
    CThing* const thing =
        scriptThing->m_pVTable->m_ResolveThing(&cameraPoint);
    if (thing == nullptr)
    {
        return;
    }

    CameraPointComponentOverlay* const camera =
        reinterpret_cast<CThingOverlay*>(thing)->m_pCameraPoint;
    if (camera == nullptr)
    {
        return;
    }

    const CRightHandedSet* const orientation =
        camera->m_pVTable->m_GetRightHandedSet(camera);
    const auto* const self =
        reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    self->m_pVTable->m_CameraUsePosition(
        this,
        cameraTarget,
        &camera->m_Position,
        orientation,
        transitionTime,
        parameter1,
        parameter2);
}
