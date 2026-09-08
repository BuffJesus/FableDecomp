#include "engine/CGameCameraManager.h"

struct CCamera { char data[0x40]; };
struct CameraState { char data[4]; };
struct CGameCameraManagerMethods : CGameCameraManager {
    virtual CCamera GetRenderCamera(float zoom) const;
    CCamera Helper6978d0(float zoom, CameraState* camera, CameraState* oldCamera) const;
};

CCamera CGameCameraManagerMethods::GetRenderCamera(float zoom) const {
    const CGameCameraManager* manager =
        reinterpret_cast<const CGameCameraManager*>(this);
    return Helper6978d0(
        zoom,
        reinterpret_cast<CameraState*>(const_cast<unsigned char*>(manager->Camera)),
        reinterpret_cast<CameraState*>(const_cast<unsigned char*>(manager->OldCamera)));
}
