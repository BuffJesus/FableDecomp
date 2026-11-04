#pragma once
#include "fable_bounding_volumes.h"
#include "fable_matrix4x4.h"
#include "engine/CMatrix3x4.h"
class CShadowClipVolume {
public:
    CPlane ClipPlanes[8];
    long ClipPlaneCount;
    C3DBoundingSphere BoundingSphere;
    bool ClipToSphere;
};
class CShadowRendererSceneData {
public:
    CShadowRendererSceneData(const CShadowRendererSceneData&);
    CMatrix4x4 WorldToShadowBufferMatrix;
    CMatrix4x4 WorldToShadowBufferMatrixHomogenousScreenSpace;
    CMatrix4x4 WorldToShadowBufferMatrixTextureSpace;
    CMatrix3x4 SpotLightFalloffMatrix;
    float ShadowFadeStart,ShadowFadeEnd;
    C3DVector ShadowLightVector,CameraPos;
    CShadowClipVolume CasterClipVolume,RecieverClipVolume;
};
FABLE_STATIC_ASSERT(sizeof(CShadowClipVolume)==152);
FABLE_STATIC_ASSERT(sizeof(CShadowRendererSceneData)==576);
FABLE_STATIC_ASSERT(offsetof(CShadowRendererSceneData,CasterClipVolume)==272);
