#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineCamera.h"
#include "engine/CMatrix3x4.h"
static __forceinline float dot(const C3DVector& a,const C3DVector& b)
{
    return a.X*b.X+a.Y*b.Y+a.Z*b.Z;
}
bool CEngine::ClipSphere(const C3DBoundingSphere& sphere) const
{
    float radius=sphere.Radius;
    C3DVector center(sphere.Center.X,sphere.Center.Y,sphere.Center.Z);
    const CEngineCamera* camera=NEngineInternal::GPCamera;
    C3DVector offset=center-camera->CameraDesc.WorldPosition;
    float depth=dot(offset,*reinterpret_cast<const C3DVector*>(&camera->LookVector_X));
    if(!(radius+depth<camera->CameraDesc.NearZPlane)
        && !(depth-radius>camera->CameraDesc.FarZPlane)
        && !(dot(offset,camera->FrustrumPlanes[2].Normal)>radius)
        && !(dot(offset,camera->FrustrumPlanes[3].Normal)>radius)
        && !(dot(offset,camera->FrustrumPlanes[0].Normal)>radius)
        && !(dot(offset,camera->FrustrumPlanes[1].Normal)>radius))return true;
    return false;
}
