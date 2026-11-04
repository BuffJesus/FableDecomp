// CAIGameCameraBase slots 2-10, retail RTTI 0125C1F4 / FableWin 041AB29C.
#include "fable_camera.h"

void CAIGameCameraBase::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) const {}

bool CAIGameCameraBase::IsPosInViewingFrustrum(const C3DVector&) const {return false;}

bool CAIGameCameraBase::Is2DAreaInViewingFrustrum(const C3DVector&,float) const {return false;}

bool CAIGameCameraBase::IsVolumeInViewingFrustrum(const C3DVector&,float) const {return false;}

bool CAIGameCameraBase::IsLineOfSightToPos(const C3DVector&) const {return false;}

float CAIGameCameraBase::GetControlAngleXY() const {return GetRenderCamera(1.0f).GetAngleXY();}

void CAIGameCameraBase::SetChangeInControlDirection() {}

bool CAIGameCameraBase::GetCameraManager(const CGameCameraManager**) const {return false;}

bool CAIGameCameraBase::GetCameraManager(CGameCameraManager**) {return false;}
