#include "engine/CEngineCamera.h"
#include "fable_engine_camera_desc.h"
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_engine_internal.h"
#include "fable_virtual_pixels.h"

void CEngineCamera::RefreshD3DTransforms()
{
    C2DBoxF window;
    GFABoxToVBox(CameraDesc.Window,window);
    NEngineInternal::GPRenderManager->SetVWindow(window);
    const CMatrix3x4& view=*reinterpret_cast<const CMatrix3x4*>(ViewTransform);
    NEngineInternal::GPRenderManager->SetViewTransform(view);
    NEngineInternal::GPRenderManager->SetPerspectiveTransform(CameraDesc.NearZPlane,CameraDesc.FarZPlane,CameraDesc.ZBufferStart,CameraDesc.ZBufferEnd);
    NEngineInternal::GPShaderRenderManager->SetViewTransform(view);
    NEngineInternal::GPShaderRenderManager->SetProjectionTransform(*reinterpret_cast<const CMatrix4x4*>(ProjectionTransform));
    NEngineInternal::GPShaderRenderManager->SetCameraPos(CameraDesc.WorldPosition);
    // CMatrix3x4 identity, in recovered constructor store order.
    float identity[12];
    identity[11]=0.0f;identity[10]=0.0f;identity[9]=0.0f;
    identity[7]=0.0f;identity[6]=0.0f;identity[5]=0.0f;
    identity[3]=0.0f;identity[2]=0.0f;identity[1]=0.0f;
    identity[8]=1.0f;identity[4]=1.0f;identity[0]=1.0f;
    const CMatrix3x4& world=*reinterpret_cast<const CMatrix3x4*>(identity);
    NEngineInternal::GPRenderManager->SetWorldTransform(world);
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(world);
}
